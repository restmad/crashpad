// Copyright 2014 The Crashpad Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CRASHPAD_MINIDUMP_MINIDUMP_MODULE_WRITER_H_
#define CRASHPAD_MINIDUMP_MINIDUMP_MODULE_WRITER_H_

#include <dbghelp.h>
#include <stdint.h>
#include <time.h>

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "minidump/minidump_extensions.h"
#include "minidump/minidump_stream_writer.h"
#include "minidump/minidump_writable.h"

namespace crashpad {

namespace internal {
class MinidumpUTF16StringWriter;
}  // namespace internal

//! \brief The base class for writers of CodeView records referenced by
//!     MINIDUMP_MODULE::CvRecord in minidump files.
class MinidumpModuleCodeViewRecordWriter : public internal::MinidumpWritable {
 public:
  virtual ~MinidumpModuleCodeViewRecordWriter();

 protected:
  MinidumpModuleCodeViewRecordWriter() : MinidumpWritable() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleCodeViewRecordWriter);
};

namespace internal {

//! \brief The base class for writers of CodeView records that serve as links to
//!     `.pdb` (program database) files.
template <typename CodeViewRecordType>
class MinidumpModuleCodeViewRecordPDBLinkWriter
    : public MinidumpModuleCodeViewRecordWriter {
 public:
  //! \brief Sets the name of the `.pdb` file being linked to.
  void SetPDBName(const std::string& pdb_name) { pdb_name_ = pdb_name; }

 protected:
  MinidumpModuleCodeViewRecordPDBLinkWriter();
  ~MinidumpModuleCodeViewRecordPDBLinkWriter() override;

  // MinidumpWritable:
  size_t SizeOfObject() override;
  bool WriteObject(FileWriterInterface* file_writer) override;

  //! \brief Returns a pointer to the raw CodeView record’s data.
  //!
  //! Subclasses can use this to set fields in their codeview records other than
  //! the `pdb_name` field.
  CodeViewRecordType* codeview_record() { return &codeview_record_; }

 private:
  CodeViewRecordType codeview_record_;
  std::string pdb_name_;

  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleCodeViewRecordPDBLinkWriter);
};

}  // namespace internal

//! \brief The writer for a MinidumpModuleCodeViewRecordPDB20 object in a
//!     minidump file.
//!
//! Most users will want MinidumpModuleCodeViewRecordPDB70Writer instead.
class MinidumpModuleCodeViewRecordPDB20Writer final
    : public internal::MinidumpModuleCodeViewRecordPDBLinkWriter<
          MinidumpModuleCodeViewRecordPDB20> {
 public:
  MinidumpModuleCodeViewRecordPDB20Writer()
      : internal::MinidumpModuleCodeViewRecordPDBLinkWriter<
            MinidumpModuleCodeViewRecordPDB20>() {}

  ~MinidumpModuleCodeViewRecordPDB20Writer() override;

  //! \brief Sets MinidumpModuleCodeViewRecordPDB20::timestamp and
  //!     MinidumpModuleCodeViewRecordPDB20::age.
  void SetTimestampAndAge(time_t timestamp, uint32_t age);

 private:
  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleCodeViewRecordPDB20Writer);
};

//! \brief The writer for a MinidumpModuleCodeViewRecordPDB70 object in a
//!     minidump file.
class MinidumpModuleCodeViewRecordPDB70Writer final
    : public internal::MinidumpModuleCodeViewRecordPDBLinkWriter<
          MinidumpModuleCodeViewRecordPDB70> {
 public:
  MinidumpModuleCodeViewRecordPDB70Writer()
      : internal::MinidumpModuleCodeViewRecordPDBLinkWriter<
            MinidumpModuleCodeViewRecordPDB70>() {}

  ~MinidumpModuleCodeViewRecordPDB70Writer() override;

  //! \brief Sets MinidumpModuleCodeViewRecordPDB70::uuid and
  //!     MinidumpModuleCodeViewRecordPDB70::age.
  void SetUUIDAndAge(const UUID& uuid, uint32_t age) {
    codeview_record()->uuid = uuid;
    codeview_record()->age = age;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleCodeViewRecordPDB70Writer);
};

//! \brief The writer for an IMAGE_DEBUG_MISC object in a minidump file.
//!
//! Most users will want MinidumpModuleCodeViewRecordPDB70Writer instead.
class MinidumpModuleMiscDebugRecordWriter final
    : public internal::MinidumpWritable {
 public:
  MinidumpModuleMiscDebugRecordWriter();
  ~MinidumpModuleMiscDebugRecordWriter() {}

  //! \brief Sets IMAGE_DEBUG_MISC::DataType.
  void SetDataType(uint32_t data_type) {
    image_debug_misc_.DataType = data_type;
  }

  //! \brief Sets IMAGE_DEBUG_MISC::Data, IMAGE_DEBUG_MISC::Length, and
  //!     IMAGE_DEBUG_MISC::Unicode.
  //!
  //! If \a utf16 is `true`, \a data will be treated as UTF-8 data and will be
  //! converted to UTF-16, and IMAGE_DEBUG_MISC::Unicode will be set to `1`.
  //! Otherwise, \a data will be used as-is and IMAGE_DEBUG_MISC::Unicode will
  //! be set to `0`.
  void SetData(const std::string& data, bool utf16);

 protected:
  // MinidumpWritable:
  bool Freeze() override;
  size_t SizeOfObject() override;
  bool WriteObject(FileWriterInterface* file_writer) override;

 private:
  IMAGE_DEBUG_MISC image_debug_misc_;
  std::string data_;
  string16 data_utf16_;

  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleMiscDebugRecordWriter);
};

//! \brief The writer for a MINIDUMP_MODULE object in a minidump file.
//!
//! Because MINIDUMP_MODULE objects only appear as elements of
//! MINIDUMP_MODULE_LIST objects, this class does not write any data on its own.
//! It makes its MINIDUMP_MODULE data available to its MinidumpModuleListWriter
//! parent, which writes it as part of a MINIDUMP_MODULE_LIST.
class MinidumpModuleWriter final : public internal::MinidumpWritable {
 public:
  MinidumpModuleWriter();
  ~MinidumpModuleWriter();

  //! \brief Returns a MINIDUMP_MODULE referencing this object’s data.
  //!
  //! This method is expected to be called by a MinidumpModuleListWriter in
  //! order to obtain a MINIDUMP_MODULE to include in its list.
  //!
  //! \note Valid in #kStateWritable.
  const MINIDUMP_MODULE* MinidumpModule() const;

  //! \brief Arranges for MINIDUMP_MODULE::ModuleNameRva to point to a
  //!     MINIDUMP_STRING containing \a name.
  //!
  //! A name is required in all MINIDUMP_MODULE objects.
  //!
  //! \note Valid in #kStateMutable.
  void SetName(const std::string& name);

  //! \brief Arranges for MINIDUMP_MODULE::CvRecord to point to a CodeView
  //!     record to be written by \a codeview_record.
  //!
  //! \a codeview_record will become a child of this object in the overall tree
  //! of internal::MinidumpWritable objects.
  //!
  //! \note Valid in #kStateMutable.
  void SetCodeViewRecord(MinidumpModuleCodeViewRecordWriter* codeview_record);

  //! \brief Arranges for MINIDUMP_MODULE::MiscRecord to point to an
  //!     IMAGE_DEBUG_MISC object to be written by \a misc_debug_record.
  //!
  //! \a misc_debug_record will become a child of this object in the overall
  //! tree of internal::MinidumpWritable objects.
  //!
  //! \note Valid in #kStateMutable.
  void SetMiscDebugRecord(
      MinidumpModuleMiscDebugRecordWriter* misc_debug_record);

  //! \brief Sets IMAGE_DEBUG_MISC::BaseOfImage.
  void SetImageBaseAddress(uint64_t image_base_address) {
    module_.BaseOfImage = image_base_address;
  }

  //! \brief Sets IMAGE_DEBUG_MISC::SizeOfImage.
  void SetImageSize(uint32_t image_size) { module_.SizeOfImage = image_size; }

  //! \brief Sets IMAGE_DEBUG_MISC::CheckSum.
  void SetChecksum(uint32_t checksum) { module_.CheckSum = checksum; }

  //! \brief Sets IMAGE_DEBUG_MISC::TimeDateStamp.
  //!
  //! \note Valid in #kStateMutable.
  void SetTimestamp(time_t timestamp);

  //! \brief Sets \ref VS_FIXEDFILEINFO::dwFileVersionMS
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileVersionMS" and \ref
  //!     VS_FIXEDFILEINFO::dwFileVersionLS
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileVersionLS".
  //!
  //! \note Valid in #kStateMutable.
  void SetFileVersion(uint16_t version_0,
                      uint16_t version_1,
                      uint16_t version_2,
                      uint16_t version_3);

  //! \brief Sets \ref VS_FIXEDFILEINFO::dwProductVersionMS
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwProductVersionMS" and \ref
  //!     VS_FIXEDFILEINFO::dwProductVersionLS
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwProductVersionLS".
  //!
  //! \note Valid in #kStateMutable.
  void SetProductVersion(uint16_t version_0,
                         uint16_t version_1,
                         uint16_t version_2,
                         uint16_t version_3);

  //! \brief Sets \ref VS_FIXEDFILEINFO::dwFileFlags
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileFlags" and \ref
  //!     VS_FIXEDFILEINFO::dwFileFlagsMask
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileFlagsMask".
  //!
  //! \note Valid in #kStateMutable.
  void SetFileFlagsAndMask(uint32_t file_flags, uint32_t file_flags_mask);

  //! \brief Sets \ref VS_FIXEDFILEINFO::dwFileOS
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileOS".
  void SetFileOS(uint32_t file_os) { module_.VersionInfo.dwFileOS = file_os; }

  //! \brief Sets \ref VS_FIXEDFILEINFO::dwFileType
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileType" and \ref
  //!     VS_FIXEDFILEINFO::dwFileSubtype
  //!     "IMAGE_DEBUG_MISC::VersionInfo::dwFileSubtype".
  void SetFileTypeAndSubtype(uint32_t file_type, uint32_t file_subtype) {
    module_.VersionInfo.dwFileType = file_type;
    module_.VersionInfo.dwFileSubtype = file_subtype;
  }

 protected:
  // MinidumpWritable:
  bool Freeze() override;
  size_t SizeOfObject() override;
  std::vector<MinidumpWritable*> Children() override;
  bool WriteObject(FileWriterInterface* file_writer) override;

 private:
  MINIDUMP_MODULE module_;
  scoped_ptr<internal::MinidumpUTF16StringWriter> name_;
  MinidumpModuleCodeViewRecordWriter* codeview_record_;  // weak
  MinidumpModuleMiscDebugRecordWriter* misc_debug_record_;  // weak

  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleWriter);
};

//! \brief The writer for a MINIDUMP_MODULE_LIST stream in a minidump file,
//!     containing a list of MINIDUMP_MODULE objects.
class MinidumpModuleListWriter final : public internal::MinidumpStreamWriter {
 public:
  MinidumpModuleListWriter();
  ~MinidumpModuleListWriter();

  //! \brief Adds a MinidumpModuleWriter to the MINIDUMP_MODULE_LIST.
  //!
  //! \a module will become a child of this object in the overall tree of
  //! internal::MinidumpWritable objects.
  //!
  //! \note Valid in #kStateMutable.
  void AddModule(MinidumpModuleWriter* module);

 protected:
  // MinidumpWritable:
  bool Freeze() override;
  size_t SizeOfObject() override;
  std::vector<MinidumpWritable*> Children() override;
  bool WriteObject(FileWriterInterface* file_writer) override;

  // MinidumpStreamWriter:
  MinidumpStreamType StreamType() const override;

 private:
  MINIDUMP_MODULE_LIST module_list_base_;
  std::vector<MinidumpModuleWriter*> modules_;  // weak

  DISALLOW_COPY_AND_ASSIGN(MinidumpModuleListWriter);
};

}  // namespace crashpad

#endif  // CRASHPAD_MINIDUMP_MINIDUMP_MODULE_WRITER_H_
