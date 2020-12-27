
//https://ttsmp3.com I use this for making the recordings

#include <SoftwareSerial.h>//connection to mp3 board

void checkForMessage();
unsigned long tcpStartTime;

unsigned long bluetoothTimeoutStart;
const int speakerPwrPin = 14;
const int LEDpin = 13;//output
const int ARDUINO_RX_PIN = 33; //should connect to TX of the Serial MP3 Player module
const int ARDUINO_TX_PIN = 32; //connect to RX of the module
SoftwareSerial mp3(ARDUINO_RX_PIN, ARDUINO_TX_PIN);
bool donePlaying = false;//so we know when done playing, then kill power to the speaker
bool silenceButtonWasPressed = false;
bool firstBoot = true;

//I forgot where this code came from, but from somewhere where you buy these mp3 players
unsigned long firstPacketTime;
char oldPacketReceived[200];
//char newPacket[200];
int trackToPlay;
int receivedPackets = 0;
bool trackWillBePlayed = false;
boolean firstPacket = false;
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
String mp3Answer;           // Answer from the MP3.
String sanswer(void);
String sbyte2hex(uint8_t b);
void playTheTrack();

/************ Command byte **************************/
#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16  // Stop playing continuously. 
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_ON  0X00
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL    0X22
#define CMD_PLAYING_N     0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

/************ Opitons **************************/
#define DEV_TF            0X02


/*********************************************************************/
