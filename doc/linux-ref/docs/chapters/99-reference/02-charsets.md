# Character Sets and Encodings

Character sets and encodings are fundamental concepts in computing that define 
how characters are represented and stored in digital form. This chapter will 
provide several character sets to help with as a quick reference for developers.

## American Standard Code for Information Interchange (ASCII)

| Decimal | Hexadecimal | Character | C escape | Notes                         |
|---------|-------------|-----------|----------|-------------------------------|
| 0       | 0x00        | NUL       | `\0`       | Null character                |
| 1       | 0x01        | SOH       |          | Start of Heading              |
| 2       | 0x02        | STX       |          | Start of Text                 |
| 3       | 0x03        | ETX       |          | End of Text                   |
| 4       | 0x04        | EOT       |          | End of Transmission           |
| 5       | 0x05        | ENQ       |          | Enquiry                       |
| 6       | 0x06        | ACK       |          | Acknowledge                   |
| 7       | 0x07        | BEL       | `\a`     | Bell                          |
| 8       | 0x08        | BS        | `\b`     | Backspace                     |
| 9       | 0x09        | HT        | `\t`     | Horizontal Tab                |
| 10      | 0x0a        | LF        | `\n`     | Line Feed                     |
| 11      | 0x0b        | VT        | `\v`     | Vertical Tab                  |
| 12      | 0x0c        | FF        | `\f`     | Form Feed                     |
| 13      | 0x0d        | CR        | `\r`     | Carriage Return               |
| 14      | 0x0e        | SO        |          | Shift Out                     |
| 15      | 0x0f        | SI        |          | Shift In                      |
| 16      | 0x10        | DLE       |          | Data Link Escape              |
| 17      | 0x11        | DC1       |          | Device Control 1              |
| 18      | 0x12        | DC2       |          | Device Control 2              |
| 19      | 0x13        | DC3       |          | Device Control 3              |
| 20      | 0x14        | DC4       |          | Device Control 4              |
| 21      | 0x15        | NAK       |          | Negative Acknowledge          |
| 22      | 0x16        | SYN       |          | Synchronous Idle              |
| 23      | 0x17        | ETB       |          | End of Transmission Block     |
| 24      | 0x18        | CAN       |          | Cancel                        |
| 25      | 0x19        | EM        |          | End of Medium                 |
| 26      | 0x1a        | SUB       |          | Substitute                    |
| 27      | 0x1b        | ESC       | `\e`     | Escape                        |
| 28      | 0x1c        | FS        |          | File Separator                |
| 29      | 0x1d        | GS        |          | Group Separator               |
| 30      | 0x1e        | RS        |          | Record Separator              |
| 31      | 0x1f        | US        |          | Unit Separator                |
| 32      | 0x20        | SPACE     |          | Space                         |
| 33      | 0x21        | !         |          | Exclamation Mark              |
| 34      | 0x22        | "         |          | Quotation Mark                |
| 35      | 0x23        | #         |          | Number Sign                   |
| 36      | 0x24        | $         |          | Dollar Sign                   |
| 37      | 0x25        | %         |          | Percent Sign                  |
| 38      | 0x26        | &         |          | Ampersand                     |
| 39      | 0x27        | '         |          | Apostrophe                    |
| 40      | 0x28        | (         |          | Left Parenthesis              |
| 41      | 0x29        | )         |          | Right Parenthesis             |
| 42      | 0x2a        | *         |          | Asterisk                      |
| 43      | 0x2b        | +         |          | Plus Sign                     |
| 44      | 0x2c        | ,         |          | Comma                         |
| 45      | 0x2d        | -         |          | Hyphen-Minus                  |
| 46      | 0x2e        | .         |          | Full Stop                     |
| 47      | 0x2f        | /         |          | Solidus                       |
| 48      | 0x30        | 0         |          |                               |
| 49      | 0x31        | 1         |          |                               |
| 50      | 0x32        | 2         |          |                               |
| 51      | 0x33        | 3         |          |                               |
| 52      | 0x34        | 4         |          |                               |
| 53      | 0x35        | 5         |          |                               |
| 54      | 0x36        | 6         |          |                               |
| 55      | 0x37        | 7         |          |                               |
| 56      | 0x38        | 8         |          |                               |
| 57      | 0x39        | 9         |          |                               |
| 58      | 0x3a        | :         |          | Colon                         |
| 59      | 0x3b        | ;         |          | Semicolon                     |
| 60      | 0x3c        | <         |          | Less-Than Sign                |
| 61      | 0x3d        | =         |          | Equals Sign                   |
| 62      | 0x3e        | >         |          | Greater-Than Sign             |
| 63      | 0x3f        | ?         |          | Question Mark                 |
| 64      | 0x40        | @         |          | At Sign                       |
| 65      | 0x41        | A         |          |                               |
| 66      | 0x42        | B         |          |                               |
| 67      | 0x43        | C         |          |                               |
| 68      | 0x44        | D         |          |                               |
| 69      | 0x45        | E         |          |                               |
| 70      | 0x46        | F         |          |                               |
| 71      | 0x47        | G         |          |                               |
| 72      | 0x48        | H         |          |                               |
| 73      | 0x49        | I         |          |                               |
| 74      | 0x4a        | J         |          |                               |
| 75      | 0x4b        | K         |          |                               |
| 76      | 0x4c        | L         |          |                               |
| 77      | 0x4d        | M         |          |                               |
| 78      | 0x4e        | N         |          |                               |
| 79      | 0x4f        | O         |          |                               |
| 80      | 0x50        | P         |          |                               |
| 81      | 0x51        | Q         |          |                               |
| 82      | 0x52        | R         |          |                               |
| 83      | 0x53        | S         |          |                               |
| 84      | 0x54        | T         |          |                               |
| 85      | 0x55        | U         |          |                               |
| 86      | 0x56        | V         |          |                               |
| 87      | 0x57        | W         |          |                               |
| 88      | 0x58        | X         |          |                               |
| 89      | 0x59        | Y         |          |                               |
| 90      | 0x5a        | Z         |          |                               |
| 91      | 0x5b        | [         |          | Left Square Bracket           |
| 92      | 0x5c        | \         | `\\`     | Backslash                     |
| 93      | 0x5d        | ]         |          | Right Square Bracket          |
| 94      | 0x5e        | ^         |          | Circumflex Accent             |
| 95      | 0x5f        | _         |          | Low Line
| 96      | 0x60        | `         |          | Grave Accent                  |
| 97      | 0x61        | a         |          |                               |
| 98      | 0x62        | b         |          |                               |
| 99      | 0x63        | c         |          |                               |
| 100     | 0x64        | d         |          |                               |
| 101     | 0x65        | e         |          |                               |
| 102     | 0x66        | f         |          |                               |
| 103     | 0x67        | g         |          |                               |
| 104     | 0x68        | h         |          |                               |
| 105     | 0x69        | i         |          |                               |
| 106     | 0x6a        | j         |          |                               |
| 107     | 0x6b        | k         |          |                               |
| 108     | 0x6c        | l         |          |                               |
| 109     | 0x6d        | m         |          |                               |
| 110     | 0x6e        | n         |          |                               |
| 111     | 0x6f        | o         |          |                               |
| 112     | 0x70        | p         |          |                               |
| 113     | 0x71        | q         |          |                               |
| 114     | 0x72        | r         |          |                               |
| 115     | 0x73        | s         |          |                               |
| 116     | 0x74        | t         |          |                               |
| 117     | 0x75        | u         |          |                               |
| 118     | 0x76        | v         |          |                               |
| 119     | 0x77        | w         |          |                               |
| 120     | 0x78        | x         |          |                               |
| 121     | 0x79        | y         |          |                               |
| 122     | 0x7a        | z         |          |                               |
| 123     | 0x7b        | {         |          | Left Curly Bracket            |
| 124     | 0x7c        | \|        |          | Vertical Bar                  |
| 125     | 0x7d        | }         |          | Right Curly Bracket           |
| 126     | 0x7e        | ~         |          | Tilde                         |
| 127     | 0x7f        | DEL       |          | Delete                        |
