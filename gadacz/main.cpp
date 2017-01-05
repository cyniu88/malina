#include<string.h>
#include<malloc.h>
#include<espeak/speak_lib.h>

int main(int argc, char* argv[] )
{
    char text[] = {"dom jest mamy"};
    espeak_Initialize(AUDIO_OUTPUT_PLAYBACK, 0, NULL, 0 );
    espeak_SetVoiceByName("pl");
    unsigned int size = 0;
    while(text[size]!='\0') size++;
    unsigned int flags=espeakCHARS_AUTO | espeakENDPAUSE;
    espeak_Synth( text, size+1, 0,POS_CHARACTER,0, flags, NULL, NULL );
    espeak_Synchronize( );
    return 0;
}
