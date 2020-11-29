#include <kos/boot/efi/cmdline.hpp>
#include <kos/boot/efi/console.hpp>
#include <kos/view.hpp>
#include <kos/array.hpp>

namespace kos::boot::efi {
  template<typename T>
  bool parse_param(view<T> argument, bool& param, view<T> name) {
    if(argument == name)
      return param = true;

    if(argument.starts_with(L"no") && argument.slice(2) == name)
      return !(param = false);

    return false;
  }

  template<typename T, size_t N>
  bool parse_param(view<T> argument, array<T, N>& param, view<T> name) {
    if(!argument.starts_with(name)
        || argument[name.size()] != L'=')
      return false;

    param = argument.slice(name.size() + 1);
    return true;
  }

  template<typename T>
  bool parse_param(view<T> argument, int& param, view<T> name) {
    array<T, 32> num{};
    if(parse_param(argument, num, name)) {
      param = num.slice().to_int();
      return true;
    }
    return false;
  }

  template<typename T, typename U, typename S, size_t N>
  bool parse_param(view<T> argument, U& param, const S (&name)[N]) {
    return parse_param(argument, param, view<S>{name}.slice(0, 1));
  }

  template<typename T>
  static auto parse_word(const T* str) -> view<T> {
      while(*str == L' ')
        str++;
      auto* word_start = str;
      while(*str && *str != L' ')
        str++;
      auto* word_end = str;
      return { word_start, word_end };
  }

  auto parse_cmdline(const wchar_t* cmd) -> cmdline {
    cmdline boot_args;
    while(*cmd) {
      const auto param = parse_word(cmd);
      cmd = param.end();

      parse_param(param, boot_args.kernel,    L"kernel");
      parse_param(param, boot_args.root,      L"root");
      parse_param(param, boot_args.initramfs, L"initramfs");
      parse_param(param, boot_args.verbosity, L"verbosity");
      parse_param(param, boot_args.debug,     L"debug");
    }

    return boot_args;
  }

  void print(const cmdline& boot_args) {
    println("{");
    println("  kernel:\t",    boot_args.kernel);
    println("  initramfs:\t", boot_args.initramfs);
    println("  root:\t\t",    boot_args.root);
    println("  verbosity:\t", boot_args.verbosity);
    println("  debug:\t",     boot_args.debug);
    print("}");
  }

}
