/* 
 * Copyright (c) 1994 by the University of Southern California
 *
 * EXPORT OF THIS SOFTWARE from the United States of America may
 *     require a specific license from the United States Government.
 *     It is the responsibility of any person or organization contemplating
 *     export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to copy, modify, and distribute
 *     this software and its documentation in source and binary forms is
 *     hereby granted, provided that any documentation or other materials
 *     related to such distribution or use acknowledge that the software
 *     was developed by the University of Southern California. 
 *
 * DISCLAIMER OF WARRANTY.  THIS SOFTWARE IS PROVIDED "AS IS".  The
 *     University of Southern California MAKES NO REPRESENTATIONS OR
 *     WARRANTIES, EXPRESS OR IMPLIED.  By way of example, but not
 *     limitation, the University of Southern California MAKES NO
 *     REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
 *     PARTICULAR PURPOSE. The University of Southern
 *     California shall not be held liable for any liability nor for any
 *     direct, indirect, or consequential damages with respect to any
 *     claim by the user or distributor of the ksu software.
 *
 * KSU was writen by:  Ari Medvinsky, ari@isi.edu
 */

#include "ksu.h"

static krb5_error_code _dbm_an_to_ln();
static krb5_error_code _username_an_to_ln();
static void auth_cleanup();

krb5_boolean fowner(FILE * fp, int uid){
struct stat sbuf;

    /*
     * For security reasons, file must be owned either by
     * the user himself, or by root.  Otherwise, don't grant access.
     */
    if (fstat(fileno(fp), &sbuf)) {
	fclose(fp);
	return(FALSE);
    }

    if ((sbuf.st_uid != uid) && sbuf.st_uid) {
	fclose(fp);
	return(FALSE);
    }

return(TRUE);
}

/*
 * Given a Kerberos principal "principal", and a local username "luser",
 * determine whether user is authorized to login according to the
 * authorization files ~luser/.k5login" and ~luser/.k5users.  Returns TRUE
 * if authorized, FALSE if not authorized.
 *
 */

krb5_error_code krb5_authorization( /* IN */
				    krb5_principal principal,
				    const char *luser,
				    char * local_realm_name, char * cmd, 
				    /* OUT */	
				    krb5_boolean * ok, char ** out_fcmd)     
{
    struct passwd *pwd;
    char * kuser;
    char *princname;
    int k5login_flag =0; 
    int k5users_flag =0; 
    krb5_boolean retbool =FALSE; 
    FILE * login_fp, * users_fp;	
    krb5_error_code retval = 0;	
    struct stat statbuf;
    struct stat st_temp;

    *ok =FALSE;	

    /* no account => no access */
    if ((pwd = getpwnam(luser)) == NULL) {
	return 0;
    }

    if (retval = krb5_unparse_name(principal, &princname)){
	return retval;	
    }


#ifdef DEBUG
	printf("principal to be authorized %s\n", princname);     	
	printf("login file: %s\n", k5login_path);
	printf("users file: %s\n", k5users_path);
#endif

  	
    k5login_flag = stat(k5login_path, &st_temp);
    k5users_flag = stat(k5users_path, &st_temp);

    /* k5login and k5users must be owned by target user or root */      
    if (!k5login_flag){  	
    	if ((login_fp = fopen(k5login_path, "r")) == NULL) {
		return 0;
    	}
	if ( fowner(login_fp, pwd->pw_uid) == FALSE){ 
		return 0;
    	}
    }	

    if (!k5users_flag){  	
    	if ((users_fp = fopen(k5users_path, "r")) == NULL) {
		return 0;
    	}
	if ( fowner(users_fp, pwd->pw_uid) == FALSE){ 
		return 0;
    	}
    }	

   if (auth_debug){ 
   	 fprintf(stderr,
		"In krb5_authorization: if auth files exist -> can access\n"); 
   }

    if (cmd){	
	if(k5users_flag){
		return 0; /* if  kusers does not exist -> done */           
	}else{
		if(retval = k5users_lookup(users_fp,princname,
					   cmd,&retbool,out_fcmd)){ 
			auth_cleanup(k5users_flag,users_fp,
				k5login_flag,login_fp, princname); 
			return retval;
		}else{
    			*ok =retbool;	
			return retval;
		}
	}
    }	

	/* if either file exists,
 	   first see if the principal is in the login in file,
	   if it's not there check the k5users file */  

   if (!k5login_flag){  

	
	if (auth_debug){
   	  fprintf(stderr,
		"In krb5_authorization: principal to be authorized %s\n",
		princname); 
	}
	if (retval = k5login_lookup( login_fp,  princname, &retbool)){ 
		auth_cleanup(k5users_flag,users_fp,
				 k5login_flag,login_fp, princname); 
		return retval;
	}


   }	

   if ((!k5users_flag) && (retbool == FALSE) ){  
	if(retval = k5users_lookup (users_fp,princname,
				    cmd, &retbool,out_fcmd)){ 
		auth_cleanup(k5users_flag,users_fp,
			k5login_flag,login_fp, princname); 
		return retval;
	}
   }	

    if ( k5login_flag && k5users_flag){

	char * kuser =  (char *) calloc (strlen(princname), sizeof(char));

#ifdef DEBUG 	
	printf("krb5_lname_file %s\n", krb5_lname_file); 
#endif

	if (!stat(krb5_lname_file, &statbuf)){
		if ((! _dbm_an_to_ln(principal, strlen(princname), kuser)) &&
	 			     (strcmp(kuser, luser) == 0)){
			retbool = TRUE; /* found the right one in db */
		}
	}
	
	if (local_realm_name && (retbool == FALSE)){ 
		char * realm;	
		int used_def = 0;

		if (!strcmp(local_realm_name, USE_DEFAULT_REALM_NAME)){

    			if (retval = krb5_get_default_realm(&realm)) {
				auth_cleanup(k5users_flag,users_fp,
					k5login_flag,login_fp, princname); 
				free(kuser);
				return(retval);
    			}
			used_def =1;
		}
		else{ realm = local_realm_name; }

		if((! _username_an_to_ln(principal,strlen(princname), kuser,
					realm)) && (strcmp(kuser,luser) == 0)){
			retbool = TRUE;
		}

		if (used_def) free (realm); 
	}
	free(kuser);
    }
	 
   *ok =retbool;	
   auth_cleanup(k5users_flag,users_fp, k5login_flag,login_fp, princname); 
   return 0;
}

/***********************************************************
k5login_lookup looks for princname in file fp. Spaces    
before the princaname (in the file ) are not ignored
spaces after the princname are ignored. If there are
any tokens after the principal name  FALSE is returned.      

***********************************************************/

krb5_error_code k5login_lookup ( FILE *fp, char * princname,
				krb5_boolean * found) {   

krb5_error_code retval;
char * line;
char * fprinc, *cmd;
char * lp;  
krb5_boolean loc_found = FALSE; 


	if (retval = get_line(fp, &line )){
		return retval;	
	}

	while (line){ 
		fprinc = get_first_token (line, &lp);

		if (fprinc && (!strcmp(princname, fprinc))){
			if( get_next_token (&lp) ){ 
				free (line);
				break;  /* nothing should follow princname*/  
			}
			else{	
				loc_found = TRUE;
				free (line);
				break;
			}
		}

		free (line);
		if (retval = get_line(fp, &line )){ return retval;}
	}


*found = loc_found;
return 0;

}

/***********************************************************
k5users_lookup looks for princname in file fp. Spaces    
before the princaname (in the file ) are not ignored
spaces after the princname are ignored. 

authorization alg: 

if princname is not found return false.       

if princname is found{  
	 if cmd == NULL then the file entry after principal         	
			name must be nothing or *     
		
	 if cmd !=NULL  then entry must be matched (* is ok)        
}


***********************************************************/
krb5_error_code k5users_lookup ( FILE *fp, char * princname, char *cmd,
				krb5_boolean * found , char ** out_fcmd) {   
krb5_error_code retval;
char * line;
char * fprinc, *fcmd;
char * lp;  
char * loc_fcmd = NULL;
krb5_boolean loc_found = FALSE; 

	if (retval = get_line(fp, &line )){
		return retval;	
	}

	while (line){ 
		fprinc = get_first_token (line, &lp);

		if (fprinc && (!strcmp(princname, fprinc))){
			fcmd = get_next_token (&lp);

			if ((fcmd) && (!strcmp(fcmd, PERMIT_ALL_COMMANDS))){
 				if (get_next_token(&lp) == NULL){ 
					loc_fcmd =cmd ? strdup(cmd): NULL; 
					loc_found = TRUE;
				}
				free (line);
				break;
			}
			
			if (cmd == NULL){
				if (fcmd == NULL){ 
					loc_found = TRUE;
				}
				free (line);
				break;
				
			}else{
				if (fcmd != NULL) {
					char * temp_rfcmd, *err;
					krb5_boolean match;					
					do {
      					    if(match_commands(fcmd,cmd,&match,
							     &temp_rfcmd, &err)){
						if (auth_debug){  
						      fprintf(stderr,"%s",err);
						}
						loc_fcmd = err;
						break;
					    }else{	
						if (match == TRUE){
							loc_fcmd = temp_rfcmd;
							loc_found = TRUE;
							break;		
						}
					    }	

					}while (fcmd = get_next_token( &lp)); 
				}
				free (line);
				break;
			}
		}

		free (line);
		if (retval = get_line(fp, &line )){ return retval;}
	}

*out_fcmd = loc_fcmd;
*found = loc_found;
return 0;

}


/***********************************************
fcmd_resolve -   
takes a command specified .k5users file and        
resolves it into a full path name.         

************************************************/

krb5_boolean fcmd_resolve(char * fcmd, char *** out_fcmd, char ** out_err){
char * out_path; 
char * err;       
char ** tmp_fcmd;
char * path_ptr, *path; 
char * lp, * tc;
krb5_boolean found = FALSE; 
int i=0;
	
	tmp_fcmd = (char **) calloc (MAX_CMD, sizeof(char *));    

	if (*fcmd == '/'){  /* must be full path */       
		tmp_fcmd[0] = strdup(fcmd);   
		tmp_fcmd[1] = NULL;   
		*out_fcmd = tmp_fcmd;
		return TRUE; 	
	}else{
		int size;
		char * cmd_path_str = "";   

 		/* must be either full path or just the cmd name */        
		if (strchr(fcmd, '/')){
			err = (char *) calloc((strlen(fcmd) +200) ,sizeof(char));
			sprintf(err,"Error: bad entry - %s in %s file, must be either full path or just the cmd name\n", fcmd, KRB5_USERS_NAME);
			*out_err = err;   
			return FALSE;
		}
		
#ifndef CMD_PATH 
		err = (char *) calloc(2*(strlen(fcmd) +200) ,sizeof(char));
		sprintf(err,"Error: bad entry - %s in %s file, since %s is just the cmd name, CMD_PATH must be defined \n", fcmd, KRB5_USERS_NAME, fcmd);
			*out_err = err;   
		return FALSE; 	
#else

	path = strdup (CMD_PATH); 
	path_ptr = path;

	while ((*path_ptr == ' ') || (*path_ptr == '\t')) path_ptr ++;    

	tc = get_first_token (path_ptr, &lp);
		
	if (! tc){   
		err = (char *) calloc((strlen(fcmd) +200) ,sizeof(char));
		sprintf(err,"Error: bad entry - %s in %s file, CMD_PATH contains no paths \n",  fcmd, KRB5_USERS_NAME);
			*out_err = err;   
		return FALSE;
	} 

	i=0;
	do{
		if (*tc != '/'){  /* must be full path */       
			err = (char *) calloc((strlen(tc) +200) ,sizeof(char));
			sprintf(err,"Error: bad path %s in CMD_PATH for %s must start with '/' \n",tc, KRB5_USERS_NAME );
			*out_err = err;   
			return FALSE;
		}

		out_path = (char *) calloc( MAXPATHLEN, sizeof (char));  
		sprintf(out_path,"%s/%s",tc, fcmd ); 

		tmp_fcmd[i] = out_path;

		i++;	

	} while(tc = get_next_token (&lp));

	tmp_fcmd[i] = NULL; 
	*out_fcmd = tmp_fcmd;
	return TRUE;

#endif /* CMD_PATH */
      }
}

/********************************************
cmd_single - checks if cmd consists of a path
	     or a single token               

********************************************/

krb5_boolean cmd_single( char * cmd ){

        if ( ( strrchr( cmd, '/')) ==  NULL){
		return TRUE;
	}else{
		return FALSE;
	}
}

/********************************************
cmd_arr_cmp_postfix - compares a command with the postfix       
         of fcmd    	 
********************************************/

int cmd_arr_cmp_postfix(char ** fcmd_arr, char * cmd){
char  * temp_fcmd;
char *ptr;
int result =1;  
int i = 0;

	while(fcmd_arr[i]){	
        	if ( (ptr = strrchr( fcmd_arr[i], '/')) ==  NULL){
               		temp_fcmd = fcmd_arr[i];
        	}else {
               		temp_fcmd = ptr + 1;
        	}

		result = strcmp (temp_fcmd, cmd);
		if (result == 0){   		
			break;				
		}
		i++;
	}

return result;


}

/**********************************************
cmd_arr_cmp - checks if cmd matches any      
              of the fcmd entries. 

**********************************************/

int cmd_arr_cmp (char ** fcmd_arr, char * cmd){
int result =1;  
int i = 0;

	while(fcmd_arr[i]){	
		result = strcmp (fcmd_arr[i], cmd);
		if (result == 0){   		
			break;				
		}
		i++;
	}
return result;
}


krb5_boolean find_first_cmd_that_exists( char ** fcmd_arr, char ** cmd_out,
					 char ** err_out){      

struct stat st_temp;  
int i = 0;
krb5_boolean retbool= FALSE;  
int j =0;
char * err;
int max_ln=0; 
int tln=0; 

	while(fcmd_arr[i]){	
		tln = strlen(fcmd_arr[i]);  
		if ( tln > max_ln) max_ln = tln;   					    
		if (!stat (fcmd_arr[i], &st_temp )){ 
			*cmd_out = strdup(fcmd_arr[i]); 
			retbool = TRUE;
			break;	
		}
		i++;
	}

if (retbool == FALSE ){ 
	err = (char *) calloc((80 +max_ln*i) ,sizeof(char));
	sprintf(err,"Error: not found -> ");
	for(j= 0; j < i; j ++){ 
		sprintf(err,"%s %s ", err, fcmd_arr[j]);
	}	
	sprintf(err,"%s\n", err);
	*err_out = err; 
}

	
return retbool;  
}

/***************************************************************
returns 1 if there is an error, 0 if no error.

***************************************************************/

int match_commands ( char * fcmd, char * cmd, krb5_boolean *match,
	             char **cmd_out, char ** err_out){ 

char ** fcmd_arr; 
char * err;  
char * cmd_temp; 

if(fcmd_resolve(fcmd, &fcmd_arr, &err )== FALSE ){
	*err_out = err; 
	return 1;	
}

if (cmd_single( cmd ) == TRUE){ 
	if (!cmd_arr_cmp_postfix(fcmd_arr, cmd)){ /* found */

		if(find_first_cmd_that_exists( fcmd_arr,&cmd_temp,&err)== TRUE){  
			*match = TRUE;
			*cmd_out = cmd_temp; 
			return 0;
		}else{
			*err_out = err;	
			return 1;
		}
	}else{
		*match = FALSE;
		return 0;
	}	
}else{
	if (!cmd_arr_cmp(fcmd_arr, cmd)){  /* found */ 
		*match = TRUE;
		*cmd_out = strdup(cmd); 
		return 0;
	} else{
		*match = FALSE;
		return 0;
	}
}

}

/*********************************************************
   get_line - returns a line of any length.  out_line
	      is set to null if eof.  
*********************************************************/

krb5_error_code get_line ( /* IN */ FILE * fp,
			    /* OUT */ char ** out_line ){ 

char * line, *r, *newline , *line_ptr;		
int chunk_count = 1; 

	line = (char *) calloc (BUFSIZ, sizeof (char ));  
	line_ptr = line;
	line[0] = '\0';

	while (( r = fgets(line_ptr, BUFSIZ , fp)) != NULL){
		if (newline = strchr(line_ptr, '\n')){
			*newline = '\0';
			break;
		}	
		else {
		       chunk_count ++;	
		       if(!( line = (char *) realloc( line,  
				chunk_count * sizeof(char) * BUFSIZ))){
					return  ENOMEM; 
			}

			line_ptr = line + (BUFSIZ -1) *( chunk_count -1) ;
		}
	}

	if ((r == NULL) && (strlen(line) == 0)) {
		 *out_line = NULL;
	} 
	else{ 
		 *out_line = line;
	}

return 0;
}

/*******************************************************  
get_first_token - 
Expects a '\0' terminated input line . 
If there are any spaces before the first token, they    
will be returned as part of the first token.        

Note: this routine reuses the space pointed to by line 
******************************************************/

char *  get_first_token (char * line, char ** lnext){
	
char * lptr, * out_ptr;

	
	out_ptr = line;    
	lptr = line;    
	
	while (( *lptr == ' ') || (*lptr == '\t')) lptr ++;  

	if (strlen(lptr) == 0) return NULL;

	while (( *lptr != ' ') && (*lptr != '\t') && (*lptr != '\0')) lptr ++;  

	if (*lptr == '\0'){
		 *lnext = lptr; 
	} else{
		*lptr = '\0';	
		*lnext = lptr + 1; 
	}

return out_ptr;
}
/**********************************************************
get_next_token - 
returns the next token pointed to by *lnext.             
returns NULL if there is no more tokens.           
Note: that this function modifies the stream        
      pointed to by *lnext and does not allocate
      space for the returned tocken. It also advances
      lnext to the next tocken.                        
**********************************************************/

char *  get_next_token (char ** lnext){
	
char * lptr, * out_ptr;

	
	lptr = *lnext;    
	
	while (( *lptr == ' ') || (*lptr == '\t')) lptr ++;  

	if (strlen(lptr) == 0) return NULL;

	out_ptr = lptr;    

	while (( *lptr != ' ') && (*lptr != '\t') && (*lptr != '\0')) lptr ++;  

	if (*lptr == '\0'){
		 *lnext = lptr; 
	} else{
		*lptr = '\0';	
		*lnext = lptr + 1; 
	}

return out_ptr;
}

/********************************************************************
 * Implementation:  This version uses a DBM database, indexed by aname,
 * to generate a lname.
 *
 * The entries in the database are normal C strings, and include the trailing
 * null in the DBM datum.size.
 ********************************************************************/
static krb5_error_code
_dbm_an_to_ln( krb5_const_principal aname, const int lnsize, char *lname)
{
    DBM *db;
    krb5_error_code retval;
    datum key, contents;
    char *princ_name;

    if (retval = krb5_unparse_name(aname, &princ_name))
	return(retval);
    key.dptr = princ_name;
    key.dsize = strlen(princ_name)+1;	/* need to store the NULL for
					   decoding */

    db = dbm_open(krb5_lname_file, O_RDONLY, 0600);
    if (!db) {
	krb5_xfree(princ_name);
	return KRB5_LNAME_CANTOPEN;
    }

    contents = dbm_fetch(db, key);

    krb5_xfree(princ_name);

    if (contents.dptr == NULL) {
	retval = KRB5_LNAME_NOTRANS;
    } else {
	strncpy(lname, contents.dptr, lnsize);
	if (lnsize < contents.dsize)
	    retval = KRB5_CONFIG_NOTENUFSPACE;
	else if (lname[contents.dsize-1] != '\0')
	    retval = KRB5_LNAME_BADFORMAT;
	else
	    retval = 0;
    }
    /* can't close until we copy the contents. */
    (void) dbm_close(db);
    return retval;
}

/*****************************************************************
 * Implementation:  This version checks the realm to see if it is the 
 * realm passed in; if so, and there is exactly one non-realm 
 * component to the name, that name is returned as the lname.
 ************************************************************/

static krb5_error_code
_username_an_to_ln ( krb5_const_principal aname, const int lnsize, 
		     char *lname, char * realm) 
{
    krb5_error_code retval;
    int realm_length;

    realm_length = krb5_princ_realm(aname)->length;
    
    if ((realm_length != strlen(realm)) ||
        (memcmp(realm, krb5_princ_realm(aname)->data, realm_length))) {
        return KRB5_LNAME_NOTRANS;
    }

    if (krb5_princ_size(aname) != 1) {
        if (krb5_princ_size(aname) == 2 ) {
           /* Check to see if 2nd component is the local realm. */
           if ( strncmp(krb5_princ_component(aname,1)->data,realm,
                        realm_length) ||
                realm_length != krb5_princ_component(aname,1)->length)
                return KRB5_LNAME_NOTRANS;
        }
        else
           /* no components or more than one component to non-realm part of name
           --no translation. */
            return KRB5_LNAME_NOTRANS;
    }

    strncpy(lname, krb5_princ_component(aname,0)->data, 
	    min(krb5_princ_component(aname,0)->length,lnsize));
    if (lnsize < krb5_princ_component(aname,0)->length ) {
	retval = KRB5_CONFIG_NOTENUFSPACE;
    } else {
	lname[krb5_princ_component(aname,0)->length] = '\0';
	retval = 0;
    }
    return retval;
}

void auth_cleanup(int k5users_flag, FILE * users_fp,
		  int k5login_flag, FILE * login_fp, char *princname){

	free (princname);
        if (!k5users_flag) fclose(users_fp);
        if (!k5login_flag) fclose(login_fp);

}

void init_auth_names(char *pw_dir){

        if ((strlen(pw_dir) == 1) && (*pw_dir == '/')){
                sprintf(k5login_path,"%s%s", pw_dir, KRB5_LOGIN_NAME);
                sprintf(k5users_path,"%s%s", pw_dir, KRB5_USERS_NAME);
        }else{
                sprintf(k5login_path,"%s/%s", pw_dir, KRB5_LOGIN_NAME);
                sprintf(k5users_path,"%s/%s", pw_dir, KRB5_USERS_NAME);
        }
}
