#ifdef __STDC__
# define	P(s) s
#else
# define P(s) ()
#endif


/* KRB5-types.c */
int encode_KRB5_Realm P((PE *, int , integer , char *, struct type_KRB5_Realm *));
int encode_KRB5_PrincipalName P((PE *, int , integer , char *, struct type_KRB5_PrincipalName *));
int encode_KRB5_HostAddress P((PE *, int , integer , char *, struct type_KRB5_HostAddress *));
int encode_KRB5_HostAddresses P((PE *, int , integer , char *, struct type_KRB5_HostAddresses *));
int encode_KRB5_AuthorizationData P((PE *, int , integer , char *, struct type_KRB5_AuthorizationData *));
int encode_KRB5_KDCOptions P((PE *, int , integer , char *, struct type_KRB5_KDCOptions *));
int encode_KRB5_LastReq P((PE *, int , integer , char *, struct type_KRB5_LastReq *));
int encode_KRB5_KerberosTime P((PE *, int , integer , char *, struct type_KRB5_KerberosTime *));
int encode_KRB5_Ticket P((PE *, int , integer , char *, struct type_KRB5_Ticket *));
int encode_KRB5_TransitedEncoding P((PE *, int , integer , char *, struct type_KRB5_TransitedEncoding *));
int encode_KRB5_EncTicketPart P((PE *, int , integer , char *, struct type_KRB5_EncTicketPart *));
int encode_KRB5_Authenticator P((PE *, int , integer , char *, struct type_KRB5_Authenticator *));
int encode_KRB5_TicketFlags P((PE *, int , integer , char *, struct type_KRB5_TicketFlags *));
int encode_KRB5_AS__REQ P((PE *, int , integer , char *, struct type_KRB5_AS__REQ *));
int encode_KRB5_TGS__REQ P((PE *, int , integer , char *, struct type_KRB5_TGS__REQ *));
int encode_KRB5_PA__DATA P((PE *, int , integer , char *, struct type_KRB5_PA__DATA *));
int encode_KRB5_KDC__REQ__BODY P((PE *, int , integer , char *, struct type_KRB5_KDC__REQ__BODY *));
int encode_KRB5_AS__REP P((PE *, int , integer , char *, struct type_KRB5_AS__REP *));
int encode_KRB5_TGS__REP P((PE *, int , integer , char *, struct type_KRB5_TGS__REP *));
int encode_KRB5_EncASRepPart P((PE *, int , integer , char *, struct type_KRB5_EncASRepPart *));
int encode_KRB5_EncTGSRepPart P((PE *, int , integer , char *, struct type_KRB5_EncTGSRepPart *));
int encode_KRB5_AP__REQ P((PE *, int , integer , char *, struct type_KRB5_AP__REQ *));
int encode_KRB5_APOptions P((PE *, int , integer , char *, struct type_KRB5_APOptions *));
int encode_KRB5_AP__REP P((PE *, int , integer , char *, struct type_KRB5_AP__REP *));
int encode_KRB5_EncAPRepPart P((PE *, int , integer , char *, struct type_KRB5_EncAPRepPart *));
int encode_KRB5_KRB__SAFE P((PE *, int , integer , char *, struct type_KRB5_KRB__SAFE *));
int encode_KRB5_KRB__SAFE__BODY P((PE *, int , integer , char *, struct type_KRB5_KRB__SAFE__BODY *));
int encode_KRB5_KRB__PRIV P((PE *, int , integer , char *, struct type_KRB5_KRB__PRIV *));
int encode_KRB5_EncKrbPrivPart P((PE *, int , integer , char *, struct type_KRB5_EncKrbPrivPart *));
int encode_KRB5_KRB__ERROR P((PE *, int , integer , char *, struct type_KRB5_KRB__ERROR *));
int encode_KRB5_EncryptedData P((PE *, int , integer , char *, struct type_KRB5_EncryptedData *));
int encode_KRB5_EncryptionKey P((PE *, int , integer , char *, struct type_KRB5_EncryptionKey *));
int encode_KRB5_Checksum P((PE *, int , integer , char *, struct type_KRB5_Checksum *));
int encode_KRB5_METHOD__DATA P((PE *, int , integer , char *, struct type_KRB5_METHOD__DATA *));
int decode_KRB5_Realm P((PE , int , integer *, char **, struct type_KRB5_Realm **));
int decode_KRB5_PrincipalName P((PE , int , integer *, char **, struct type_KRB5_PrincipalName **));
int decode_KRB5_HostAddress P((PE , int , integer *, char **, struct type_KRB5_HostAddress **));
int decode_KRB5_HostAddresses P((PE , int , integer *, char **, struct type_KRB5_HostAddresses **));
int decode_KRB5_AuthorizationData P((PE , int , integer *, char **, struct type_KRB5_AuthorizationData **));
int decode_KRB5_KDCOptions P((PE , int , integer *, char **, struct type_KRB5_KDCOptions **));
int decode_KRB5_LastReq P((PE , int , integer *, char **, struct type_KRB5_LastReq **));
int decode_KRB5_KerberosTime P((PE , int , integer *, char **, struct type_KRB5_KerberosTime **));
int decode_KRB5_Ticket P((PE , int , integer *, char **, struct type_KRB5_Ticket **));
int decode_KRB5_TransitedEncoding P((PE , int , integer *, char **, struct type_KRB5_TransitedEncoding **));
int decode_KRB5_EncTicketPart P((PE , int , integer *, char **, struct type_KRB5_EncTicketPart **));
int decode_KRB5_Authenticator P((PE , int , integer *, char **, struct type_KRB5_Authenticator **));
int decode_KRB5_TicketFlags P((PE , int , integer *, char **, struct type_KRB5_TicketFlags **));
int decode_KRB5_AS__REQ P((PE , int , integer *, char **, struct type_KRB5_AS__REQ **));
int decode_KRB5_TGS__REQ P((PE , int , integer *, char **, struct type_KRB5_TGS__REQ **));
int decode_KRB5_PA__DATA P((PE , int , integer *, char **, struct type_KRB5_PA__DATA **));
int decode_KRB5_KDC__REQ__BODY P((PE , int , integer *, char **, struct type_KRB5_KDC__REQ__BODY **));
int decode_KRB5_AS__REP P((PE , int , integer *, char **, struct type_KRB5_AS__REP **));
int decode_KRB5_TGS__REP P((PE , int , integer *, char **, struct type_KRB5_TGS__REP **));
int decode_KRB5_EncASRepPart P((PE , int , integer *, char **, struct type_KRB5_EncASRepPart **));
int decode_KRB5_EncTGSRepPart P((PE , int , integer *, char **, struct type_KRB5_EncTGSRepPart **));
int decode_KRB5_AP__REQ P((PE , int , integer *, char **, struct type_KRB5_AP__REQ **));
int decode_KRB5_APOptions P((PE , int , integer *, char **, struct type_KRB5_APOptions **));
int decode_KRB5_AP__REP P((PE , int , integer *, char **, struct type_KRB5_AP__REP **));
int decode_KRB5_EncAPRepPart P((PE , int , integer *, char **, struct type_KRB5_EncAPRepPart **));
int decode_KRB5_KRB__SAFE P((PE , int , integer *, char **, struct type_KRB5_KRB__SAFE **));
int decode_KRB5_KRB__SAFE__BODY P((PE , int , integer *, char **, struct type_KRB5_KRB__SAFE__BODY **));
int decode_KRB5_KRB__PRIV P((PE , int , integer *, char **, struct type_KRB5_KRB__PRIV **));
int decode_KRB5_EncKrbPrivPart P((PE , int , integer *, char **, struct type_KRB5_EncKrbPrivPart **));
int decode_KRB5_KRB__ERROR P((PE , int , integer *, char **, struct type_KRB5_KRB__ERROR **));
int decode_KRB5_EncryptedData P((PE , int , integer *, char **, struct type_KRB5_EncryptedData **));
int decode_KRB5_EncryptionKey P((PE , int , integer *, char **, struct type_KRB5_EncryptionKey **));
int decode_KRB5_Checksum P((PE , int , integer *, char **, struct type_KRB5_Checksum **));
int decode_KRB5_METHOD__DATA P((PE , int , integer *, char **, struct type_KRB5_METHOD__DATA **));
int free_KRB5_PrincipalName P((struct type_KRB5_PrincipalName *));
int free_KRB5_HostAddress P((struct type_KRB5_HostAddress *));
int free_KRB5_HostAddresses P((struct type_KRB5_HostAddresses *));
int free_KRB5_AuthorizationData P((struct type_KRB5_AuthorizationData *));
int free_KRB5_LastReq P((struct type_KRB5_LastReq *));
int free_KRB5_Ticket P((struct type_KRB5_Ticket *));
int free_KRB5_TransitedEncoding P((struct type_KRB5_TransitedEncoding *));
int free_KRB5_EncTicketPart P((struct type_KRB5_EncTicketPart *));
int free_KRB5_Authenticator P((struct type_KRB5_Authenticator *));
int free_KRB5_AS__REQ P((struct type_KRB5_AS__REQ *));
int free_KRB5_TGS__REQ P((struct type_KRB5_TGS__REQ *));
int free_KRB5_PA__DATA P((struct type_KRB5_PA__DATA *));
int free_KRB5_KDC__REQ__BODY P((struct type_KRB5_KDC__REQ__BODY *));
int free_KRB5_AS__REP P((struct type_KRB5_AS__REP *));
int free_KRB5_TGS__REP P((struct type_KRB5_TGS__REP *));
int free_KRB5_EncASRepPart P((struct type_KRB5_EncASRepPart *));
int free_KRB5_EncTGSRepPart P((struct type_KRB5_EncTGSRepPart *));
int free_KRB5_AP__REQ P((struct type_KRB5_AP__REQ *));
int free_KRB5_AP__REP P((struct type_KRB5_AP__REP *));
int free_KRB5_EncAPRepPart P((struct type_KRB5_EncAPRepPart *));
int free_KRB5_KRB__SAFE P((struct type_KRB5_KRB__SAFE *));
int free_KRB5_KRB__SAFE__BODY P((struct type_KRB5_KRB__SAFE__BODY *));
int free_KRB5_KRB__PRIV P((struct type_KRB5_KRB__PRIV *));
int free_KRB5_EncKrbPrivPart P((struct type_KRB5_EncKrbPrivPart *));
int free_KRB5_KRB__ERROR P((struct type_KRB5_KRB__ERROR *));
int free_KRB5_EncryptedData P((struct type_KRB5_EncryptedData *));
int free_KRB5_EncryptionKey P((struct type_KRB5_EncryptionKey *));
int free_KRB5_Checksum P((struct type_KRB5_Checksum *));
int free_KRB5_METHOD__DATA P((struct type_KRB5_METHOD__DATA *));

#undef P
