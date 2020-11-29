#pragma once

#include <kos/types.hpp>
#include <kos/array.hpp>

namespace kos::boot::efi {

#pragma pack(push, 1)
  struct alignas(64) gnu_tar_header {
    array<u8, 100> name;
    array<u8, 8>   mode;
    array<u8, 8>   uid;
    array<u8, 8>   gid;
    array<s8, 12>  size;
    array<s8, 12>  mtime;
    array<u8, 8>   checksum;
    u8 typeflag;
    array<u8, 100> linkname;
    array<s8, 6> magic;
    u16 version;
    array<s8, 32> uname;
    array<s8, 32> gname;
    u64 devmajor;
    u64 devminor;
    array<s8, 12> atime;
    array<s8, 12> ctime;
    array<u8, 12> offset;
    u32 longnames;
    u8 unused;
    struct {
      array<u8, 12> offset;
      array<u8, 12> numbytes;
    } sparse[4];
    u8 isextended;
    array<u8, 12> realsize;
  };
#pragma pack(pop)

  static_assert(sizeof(gnu_tar_header) == 512);

  struct tar_file {
    gnu_tar_header& header;
    view<u8> data;
  };

  struct tar_archive {
    view<u8> archive;

    struct iterator {
      const gnu_tar_header* ptr;

      auto operator*()  -> tar_file&;
      auto operator->() -> tar_file&;
      auto operator++() -> iterator;
      auto operator++(int) -> iterator;
      auto operator!=(const iterator&) -> bool;
      auto operator==(const iterator&) -> bool;
      // TODO: operator spaceship
    };

    auto begin() const -> iterator;
    auto end() const -> iterator;
  };

  bool valid(const tar_file&);
  bool valid(const tar_archive&);

}
