#include <optional>
#include <string>
class Console {
public:
  bool isOpen() const;
  void toggle();

  bool onKey(int key);
  std::optional<std::string> pollCommand();

private:
  bool open = false;
  std::string buffer;
};
