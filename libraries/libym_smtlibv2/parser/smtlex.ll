
DEGIT   [0-9]
DEGIT1  [1-9]
BINCHAR [01]
HEXCHAR [0-9a-fA-F]
STRCHAR [a-zA-Z~!@$%^&*_+=<>.?/-]

%%

0|({DEGIT1}{DEGIT}*) {

}

(0|({DEGIT1}{DEGIT}*))"."({DEGIT}+) {

}

0b({BINCHAR}+) {

}

0x({HEXCHAR}+) {

}

{STRCHAR}(({DEGIT}|{STRCHAR})*) {

%%
