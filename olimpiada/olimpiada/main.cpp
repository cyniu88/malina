// #include <festival.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

enum CARD { _2 = 2, _3, _4, _5, _6, _7, _8, _9, _10 = 10, J = 11, D = 12, K = 13, A = 14 };
inline std::ostream &operator<<(std::ostream &os, const CARD cat)
{
  switch (cat) {
    case _2:
      os << " 2";
      break;

    case _3:
      os << " 3";
      break;

    case _4:
      os << " 4";
      break;

    case _5:
      os << " 5";
      break;

    case _6:
      os << " 6";
      break;

    case _7:
      os << " 7";
      break;

    case _8:
      os << " 8";
      break;

    case _9:
      os << " 9";
      break;
    case _10:
      os << "10";
      break;
    case J:
      os << " J";
      break;
    case D:
      os << " D";
      break;
    case K:
      os << " K";
      break;
    case A:
      os << " A";
      break;
    }
  return os;
}

int run()
{
  std::random_device rd;
  std::mt19937 e(rd());

  std::vector<CARD> db;
  for (int i = 0; i < 4; ++i) {
      db.push_back(CARD::A);
      db.push_back(CARD::K);
      db.push_back(CARD::D);
      db.push_back(CARD::J);
      db.push_back(CARD::_10);
      db.push_back(CARD::_9);
      db.push_back(CARD::_8);
      db.push_back(CARD::_7);
      db.push_back(CARD::_6);
      db.push_back(CARD::_5);
      db.push_back(CARD::_4);
      db.push_back(CARD::_3);
      db.push_back(CARD::_2);
    }

  std::shuffle(db.begin(), db.end(), e);

  //    for (const auto &d : db)
  //        std::cout << d << " ";

  //    std::cout << std::endl << "size : " << db.size() << std::endl;

  std::vector<CARD> playerA(db.begin(), db.begin() + (db.size() / 2));
  std::vector<CARD> playerB(db.begin() + (db.size() / 2), db.end());
  std::vector<CARD> pula;

  //    for (const auto &d : playerA)
  //        std::cout << d << " ";

  //    std::cout << std::endl << "size : " << playerA.size() << std::endl;

  //    for (const auto &d : playerB)
  //        std::cout << d << " ";

  //    std::cout << std::endl << "size : " << playerB.size() << std::endl;

  int iteracja = 0;
  do {
      ++iteracja;

      // std::cout << playerA.at(0) << " vs " << playerB.at(0) << std::endl;

      if (playerA.at(0) > playerB.at(0)) {
          playerA.push_back(playerB.at(0));
          std::rotate(playerA.begin(), playerA.begin() + 1, playerA.end());
          std::copy(pula.cbegin(), pula.cend(), std::back_inserter(playerA));
          pula.clear();
          playerB.erase(playerB.begin());
        } else if (playerA.at(0) < playerB.at(0)) {
          playerB.push_back(playerA.at(0));
          std::rotate(playerB.begin(), playerB.begin() + 1, playerB.end());

          std::copy(pula.cbegin(), pula.cend(), std::back_inserter(playerB));
          pula.clear();
          playerA.erase(playerA.begin());
        } else {
          pula.push_back(playerB.at(0));
          pula.push_back(playerA.at(0));
          playerB.erase(playerB.begin());
          playerA.erase(playerA.begin());

          if (not playerA.empty() and not playerB.empty())
            {
              pula.push_back(playerB.at(0));
              pula.push_back(playerA.at(0));
              playerB.erase(playerB.begin());
              playerA.erase(playerA.begin());
            }
        }

      //        for (const auto &d : playerA)
      //            std::cout << d << " ";
      //        std::cout << std::endl << "size : " << playerA.size() <<
      //        std::endl; for (const auto &d : playerB)
      //            std::cout << d << " ";
      //        std::cout << std::endl << "size : " << playerB.size() <<
      //        std::endl; std::cout << "##############################" <<
      //        std::endl;

    } while (not playerA.empty() and not playerB.empty());
  //    for (const auto &d : playerA)
  //        std::cout << d << " ";
  //    std::cout << std::endl << "size : " << playerA.size() << std::endl;
  //    for (const auto &d : playerB)
  //        std::cout << d << " ";
  //    std::cout << std::endl << "size : " << playerB.size() << std::endl;

  //    std::cout << "zajelo to iteracji: " << iteracja << std::endl;

  return iteracja;
}

int main()
{
  int max = 0;
  int min = 10'000;
  int run_m = 100'000;

  for (int i = 0; i < run_m; ++i) {
      auto res = run();
      if (res < min)
        min = res;
      if (res > max)
        max = res;
    }

  std::cout << "koniec gry,  minimalna gra: " << min << " maksymalna ilosc gier: " << max
            << std::endl;
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
