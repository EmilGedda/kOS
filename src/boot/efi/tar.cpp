#include <kos/boot/efi/tar.hpp>

namespace kos::boot::efi {

  auto tar_archive::begin() const -> tar_archive::iterator {
    return { reinterpret_cast<const gnu_tar_header*>(archive.begin()) };
  }

  auto tar_archive::end() const -> tar_archive::iterator {
    return { reinterpret_cast<const gnu_tar_header*>(archive.end() - 1024) };
  }

  bool valid(const tar_file& file) {
    if (file.header.magic != array<char,7>{"ustar "})
      return false;
    return true;
  }

  bool valid(const tar_archive& tar) {
    auto archive = tar.archive;
    auto length = archive.size();

    // verify that the archive is a whole number of records
    if(length % 512)
      return false;

    // need at least two records for the trailer
    if(length < 1024)
      return false;

    // verify sanity of all files
    for(const tar_file& file: tar)
      if (!valid(file))
        return false;

    // verify that the archive has two empty trailing records
    for(auto byte: archive.slice(length - 1024))
      if(byte != 0)
        return false;

    return true;
  }
};
