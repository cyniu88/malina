// #include <festival.h>

#include <iostream>
#include "include/influxm/client.h"

#define ASSERT_TRUE(v)                                         \
  do                                                           \
  {                                                            \
    if (!(v))                                                  \
    {                                                          \
      printf("bad assertion: %s:%d: " #v, __func__, __LINE__); \
      return -1;                                               \
    }                                                          \
  } while (0)

int main()
{

  std::cout << "start " << std::endl;
  char points[4096];
  int pointsSize = 4096, offset = 0;

  influx_client::flux::Client client(
      "10.9.0.34", /* port */ 8086, /* token */
      " "
      "-aaaapov11112lj2-ovr5bbbbso6q==",
      "organization", "iDom");

  /* do something with client */

  auto code = client.writes(
      {
          {
              "temperatura",
              {},
              {{"value", 1250}, {"value1", 1250}},
              0,
          },
          {
              "temperatura2",
              {},
              {{"value2", 1350}, {"value12", 150}},
              0,
          },
      },
      points, pointsSize);

  std::cout << "zwracany kod " << code << std::endl;
  ASSERT_TRUE(code == 204);

  return 0;
}

//{
//    festival_initialize(2, 210000);
//    festival_eval_command("(voice_kal_diphone)");
//    //festival_say_file("tekst.txt");
//    //    festival_text_to_wave("To jest przykład tekstu, który zostanie
//    odtworzony jako mowa.",
//    //                          "plik.wav");
//    festival_say_text("witaj swiecie");
//    festival_wait_for_spooler();
//    festival_tidy_up();
//    return 0;
//}
