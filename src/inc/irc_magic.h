#ifndef __IRC_MAGIC_H
#define __IRC_MAGIC_H

#define COMMAND_LEN     64      // unconfirm
#define PARAM_COUNT     64      // unconfirm
#define PARAM_LEN       64      // unconfirm

#define CHAN_PREFIX1    '#'
#define CHAN_PREFIX2    '&'
#define IS_CHAN(str) ((str)[0] == CHAN_PREFIX1 || (str)[0] == CHAN_PREFIX2)

#define RPL_WELCOME         1
#define RPL_YOURHOST        2
#define RPL_CREATED         3
#define RPL_MYINFO          4
#define RPL_BOUNCE          5

/* Replies generated in the response to commands are found in the range from 200 to 399 */
#define RPL_TRACELINK       200
#define RPL_TRACECONNECTING 201
#define RPL_TRACEHANDSHAKE  202
#define RPL_TRACEUNKNOWN    203
#define RPL_TRACEOPERATOR   204
#define RPL_TRACEUSER       205
#define RPL_TRACESERVER     206
#define RPL_TRACESERVICE    207
#define RPL_TRACENEWTYPE    208
#define RPL_TRACECLASS      209
#define RPL_TRACERECONNECT  210    //Unused
#define RPL_STATSLINKINFO   211
#define RPL_STATSCOMMANDS   212
#define RPL_ENDOFSTATS      219
#define RPL_UMODEIS         221
#define RPL_SERVLIST        234
#define RPL_SERVLISTEND     235
#define RPL_STATSUPTIME     242
#define RPL_STATSOLINE      243
#define RPL_LUSERCLIENT     251
#define RPL_LUSEROP         252
#define RPL_LUSERUNKNOWN    253
#define RPL_LUSERCHANNELS   254
#define RPL_LUSERME         255
#define RPL_ADMINME         256
#define RPL_ADMINLOC1       257
#define RPL_ADMINLOC2       258
#define RPL_ADMINEMAIL      259
#define RPL_TRACELOG        261
#define RPL_TRACEEND        262
#define RPL_TRYAGAIN        263
#define RPL_AWAY            301
#define RPL_USERHOST        302
#define RPL_ISON            303
#define RPL_UNAWAY          305
#define RPL_NOWAWAY         306
#define RPL_WHOISUSER       311
#define RPL_WHOISSERVER     312
#define RPL_WHOISOPERATOR   313
#define RPL_WHOWASUSER      314
#define RPL_ENDOFWHO        315
#define RPL_WHOISIDLE       317
#define RPL_ENDOFWHOIS      318
#define RPL_WHOISCHANNELS   319
#define RPL_LISTSTART       321
#define RPL_LIST            322
#define RPL_LISTEND         323
#define RPL_CHANNELMODEIS   324
#define RPL_UNIQOPIS        325
#define RPL_NOTOPIC         331
#define RPL_TOPIC           332
#define RPL_INVITING        341
#define RPL_SUMMONING       342
#define RPL_INVITELIST      346
#define RPL_ENDOFINVITELIST 347
#define RPL_EXCEPTLIST      348
#define RPL_ENDOFEXCEPTLIST 349
#define RPL_VERSION         351
#define RPL_WHOREPLY        352
#define RPL_NAMREPLY        353
#define RPL_LINKS           364
#define RPL_ENDOFLINKS      365
#define RPL_ENDOFNAMES      366
#define RPL_BANLIST         367
#define RPL_ENDOFBANLIST    368
#define RPL_ENDOFWHOWAS     369
#define RPL_INFO            371
#define RPL_MOTD            372
#define RPL_ENDOFINFO       374
#define RPL_MOTDSTART       375
#define RPL_ENDOFMOTD       376
#define RPL_YOUREOPER       381
#define RPL_REHASHING       382
#define RPL_YOURESERVICE    383
#define RPL_TIME            391
#define RPL_USERSSTART      392
#define RPL_USERS           393
#define RPL_ENDOFUSERS      394
#define RPL_NOUSERS         395

#endif /* __IRC_MAGIC_H */
