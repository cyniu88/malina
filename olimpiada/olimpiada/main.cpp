// #include <festival.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include <execinfo.h>
#include <signal.h>
#include <cxxabi.h> // demangle

void handleCrash(int sig)
{

    void* array[100];

    // Signal catched
    fprintf(stderr, "handleCrash() signal received is: %d\n", sig);

    // Backtrace available
    int n_addresses = backtrace(array, 100);
    fprintf(stderr, "backtrace() returned addresses: %d\n", n_addresses);

    // Instead of 'backtrace_symbols_fd(array, n_addresses, STDERR_FILENO)', we will iterate to demangle symbols:
    char** addresses = backtrace_symbols(array, n_addresses);

    if (addresses == NULL)
    {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    // Demangle backtrace addresses:
    std::string address, symbol;
    // This check is removed.
    // cppcheck-suppress variableScope
    size_t openPos, closePos, plusPos;
    char* demangledName;
    int status;

    for (int k = 0; k < n_addresses; k++)
    {
        // Address example: ./a.out(_Z16displayBacktracev) [0x556df272d3f6]
        // Need to extract the symbol within brackets:
        address = addresses[k];
        openPos = address.find("(") + 1;
        closePos = address.find(")");

        if (closePos > openPos)
        {
            symbol = address.substr(openPos, closePos - openPos);
            plusPos = symbol.find("+");

            if (plusPos > 1)
            {
                symbol = symbol.substr(0, plusPos);
            }

            status = -1;
            demangledName = abi::__cxa_demangle(symbol.c_str(), NULL, NULL, &status );

            if (status == 0)
            {
                fprintf(stderr, "%s\n", demangledName);
            }
            else
            {
                fprintf(stderr, "%s\n", address.c_str());
            }

            free( demangledName );
        }
        else
        {
            fprintf(stderr, "%s\n", address.c_str());
        }
    }

    free(addresses);
}


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
void kokos(){

            raise(SIGABRT);
}

int main()
{
   signal(SIGABRT, handleCrash);
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
  kokos();

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
