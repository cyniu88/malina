#ifndef THERMOMETER_CONTAINER_H
#define THERMOMETER_CONTAINER_H
#include <map>
#include <vector>

#include "../../libs/Statistic/statistic.h"
#include "../idom_api.h"

enum class TEMPERATURE_STATE
{
  Under,
  Over,
  NoChanges,
  Unknown
};

inline std::ostream &operator<<(std::ostream &os, const TEMPERATURE_STATE &v)
{
    switch (v)
    {
    case TEMPERATURE_STATE::Under:
        os << "Under";
        break;
    case TEMPERATURE_STATE::Over:
        os << "Over";
        break;
    case TEMPERATURE_STATE::NoChanges:
        os << "NoChanges";
        break;
    default:
        os << "Unknown";
    }
    return os;
};

struct temperature
{
  // temperature(std::string name) : thermometrName(name){};
  double newTemp = 1.0101;
  double oldTemp = 1.0101;
  double maxValue = 0;
  std::string maxDate;
  double minValue = 0;
  std::string minDate;
  std::string thermometrName;

  TEMPERATURE_STATE lastState = TEMPERATURE_STATE::Unknown;
};
class THERMOMETER : public iDom_API
{
  // THERMOMETER() = default;
public:
  explicit THERMOMETER(int iter);
  THERMOMETER(const THERMOMETER &a);
  ~THERMOMETER();

  temperature m_thermometer;
  STATISTIC<double> m_stats;

  std::string dump() const override;
};

class THERMOMETER_CONTAINER
{
#ifdef BT_TEST
public:
#endif
  std::map<std::string, THERMOMETER> thermoMap;
  THERMOMETER *returnThermometerPtr(const std::string &name);

public:
  THERMOMETER_CONTAINER() = default;
  void add(const std::string &name);
  void setTemp(const std::string &name, double value);
  double getTemp(const std::string &name);
  double getOldTemp(const std::string &name);
  TEMPERATURE_STATE getLastState(const std::string &name);
  void setState(const std::string &name, TEMPERATURE_STATE state);
  void updateAll(std::vector<std::string> *vectorThermo);
  void updateStats(const std::string &name);
  std::string getStatsByName(const std::string &name);

  bool isMoreDiff(const std::string &name, double diff);
  std::pair<double, double> getLast2(const std::string &name);
  int sizeOf();
  void showAll() const;
};

#endif // THERMOMETER_CONTAINER_H
