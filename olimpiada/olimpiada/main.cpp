#include <festival.h>

int main(int argc, char **argv)
{
    festival_initialize(2, 210000);
    festival_eval_command("(voice_kal_diphone)");
    //festival_say_file("tekst.txt");
    //    festival_text_to_wave("To jest przykład tekstu, który zostanie odtworzony jako mowa.",
    //                          "plik.wav");
    festival_say_text("witaj swiecie");
    festival_wait_for_spooler();
    festival_tidy_up();
    return 0;
}
