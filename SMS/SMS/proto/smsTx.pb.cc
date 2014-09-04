// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: smsTx.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "smsTx.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace {

const ::google::protobuf::Descriptor* BdfsMsg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BdfsMsg_reflection_ = NULL;
const ::google::protobuf::Descriptor* FsfkMsg_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  FsfkMsg_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_smsTx_2eproto() {
  protobuf_AddDesc_smsTx_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "smsTx.proto");
  GOOGLE_CHECK(file != NULL);
  BdfsMsg_descriptor_ = file->message_type(0);
  static const int BdfsMsg_offsets_[6] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, nserialid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, nsourceaddress_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, ndestaddress_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, nmsgtype_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, ninfolen_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, sinfobuff_),
  };
  BdfsMsg_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BdfsMsg_descriptor_,
      BdfsMsg::default_instance_,
      BdfsMsg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BdfsMsg, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BdfsMsg));
  FsfkMsg_descriptor_ = file->message_type(1);
  static const int FsfkMsg_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FsfkMsg, nserialid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FsfkMsg, nres_),
  };
  FsfkMsg_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      FsfkMsg_descriptor_,
      FsfkMsg::default_instance_,
      FsfkMsg_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FsfkMsg, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(FsfkMsg, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(FsfkMsg));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_smsTx_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BdfsMsg_descriptor_, &BdfsMsg::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    FsfkMsg_descriptor_, &FsfkMsg::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_smsTx_2eproto() {
  delete BdfsMsg::default_instance_;
  delete BdfsMsg_reflection_;
  delete FsfkMsg::default_instance_;
  delete FsfkMsg_reflection_;
}

void protobuf_AddDesc_smsTx_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\013smsTx.proto\"\201\001\n\007BdfsMsg\022\021\n\tnSerialId\030\001"
    " \002(\003\022\026\n\016nSourceAddress\030\002 \002(\003\022\024\n\014nDestAdd"
    "ress\030\003 \002(\003\022\020\n\010nMsgType\030\004 \002(\005\022\020\n\010nInfoLen"
    "\030\005 \002(\005\022\021\n\tsInfoBuff\030\006 \002(\t\"*\n\007FsfkMsg\022\021\n\t"
    "nSerialId\030\001 \002(\003\022\014\n\004nRes\030\002 \002(\005", 189);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "smsTx.proto", &protobuf_RegisterTypes);
  BdfsMsg::default_instance_ = new BdfsMsg();
  FsfkMsg::default_instance_ = new FsfkMsg();
  BdfsMsg::default_instance_->InitAsDefaultInstance();
  FsfkMsg::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_smsTx_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_smsTx_2eproto {
  StaticDescriptorInitializer_smsTx_2eproto() {
    protobuf_AddDesc_smsTx_2eproto();
  }
} static_descriptor_initializer_smsTx_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int BdfsMsg::kNSerialIdFieldNumber;
const int BdfsMsg::kNSourceAddressFieldNumber;
const int BdfsMsg::kNDestAddressFieldNumber;
const int BdfsMsg::kNMsgTypeFieldNumber;
const int BdfsMsg::kNInfoLenFieldNumber;
const int BdfsMsg::kSInfoBuffFieldNumber;
#endif  // !_MSC_VER

BdfsMsg::BdfsMsg()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BdfsMsg::InitAsDefaultInstance() {
}

BdfsMsg::BdfsMsg(const BdfsMsg& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BdfsMsg::SharedCtor() {
  _cached_size_ = 0;
  nserialid_ = GOOGLE_LONGLONG(0);
  nsourceaddress_ = GOOGLE_LONGLONG(0);
  ndestaddress_ = GOOGLE_LONGLONG(0);
  nmsgtype_ = 0;
  ninfolen_ = 0;
  sinfobuff_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BdfsMsg::~BdfsMsg() {
  SharedDtor();
}

void BdfsMsg::SharedDtor() {
  if (sinfobuff_ != &::google::protobuf::internal::kEmptyString) {
    delete sinfobuff_;
  }
  if (this != default_instance_) {
  }
}

void BdfsMsg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BdfsMsg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BdfsMsg_descriptor_;
}

const BdfsMsg& BdfsMsg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_smsTx_2eproto();
  return *default_instance_;
}

BdfsMsg* BdfsMsg::default_instance_ = NULL;

BdfsMsg* BdfsMsg::New() const {
  return new BdfsMsg;
}

void BdfsMsg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    nserialid_ = GOOGLE_LONGLONG(0);
    nsourceaddress_ = GOOGLE_LONGLONG(0);
    ndestaddress_ = GOOGLE_LONGLONG(0);
    nmsgtype_ = 0;
    ninfolen_ = 0;
    if (has_sinfobuff()) {
      if (sinfobuff_ != &::google::protobuf::internal::kEmptyString) {
        sinfobuff_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BdfsMsg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int64 nSerialId = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &nserialid_)));
          set_has_nserialid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_nSourceAddress;
        break;
      }

      // required int64 nSourceAddress = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nSourceAddress:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &nsourceaddress_)));
          set_has_nsourceaddress();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_nDestAddress;
        break;
      }

      // required int64 nDestAddress = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nDestAddress:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &ndestaddress_)));
          set_has_ndestaddress();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_nMsgType;
        break;
      }

      // required int32 nMsgType = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nMsgType:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &nmsgtype_)));
          set_has_nmsgtype();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_nInfoLen;
        break;
      }

      // required int32 nInfoLen = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nInfoLen:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ninfolen_)));
          set_has_ninfolen();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_sInfoBuff;
        break;
      }

      // required string sInfoBuff = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_sInfoBuff:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_sinfobuff()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->sinfobuff().data(), this->sinfobuff().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BdfsMsg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int64 nSerialId = 1;
  if (has_nserialid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->nserialid(), output);
  }

  // required int64 nSourceAddress = 2;
  if (has_nsourceaddress()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(2, this->nsourceaddress(), output);
  }

  // required int64 nDestAddress = 3;
  if (has_ndestaddress()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(3, this->ndestaddress(), output);
  }

  // required int32 nMsgType = 4;
  if (has_nmsgtype()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->nmsgtype(), output);
  }

  // required int32 nInfoLen = 5;
  if (has_ninfolen()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(5, this->ninfolen(), output);
  }

  // required string sInfoBuff = 6;
  if (has_sinfobuff()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->sinfobuff().data(), this->sinfobuff().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      6, this->sinfobuff(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BdfsMsg::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int64 nSerialId = 1;
  if (has_nserialid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->nserialid(), target);
  }

  // required int64 nSourceAddress = 2;
  if (has_nsourceaddress()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(2, this->nsourceaddress(), target);
  }

  // required int64 nDestAddress = 3;
  if (has_ndestaddress()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(3, this->ndestaddress(), target);
  }

  // required int32 nMsgType = 4;
  if (has_nmsgtype()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->nmsgtype(), target);
  }

  // required int32 nInfoLen = 5;
  if (has_ninfolen()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(5, this->ninfolen(), target);
  }

  // required string sInfoBuff = 6;
  if (has_sinfobuff()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->sinfobuff().data(), this->sinfobuff().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        6, this->sinfobuff(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int BdfsMsg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int64 nSerialId = 1;
    if (has_nserialid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->nserialid());
    }

    // required int64 nSourceAddress = 2;
    if (has_nsourceaddress()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->nsourceaddress());
    }

    // required int64 nDestAddress = 3;
    if (has_ndestaddress()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->ndestaddress());
    }

    // required int32 nMsgType = 4;
    if (has_nmsgtype()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->nmsgtype());
    }

    // required int32 nInfoLen = 5;
    if (has_ninfolen()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ninfolen());
    }

    // required string sInfoBuff = 6;
    if (has_sinfobuff()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->sinfobuff());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BdfsMsg::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BdfsMsg* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BdfsMsg*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BdfsMsg::MergeFrom(const BdfsMsg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_nserialid()) {
      set_nserialid(from.nserialid());
    }
    if (from.has_nsourceaddress()) {
      set_nsourceaddress(from.nsourceaddress());
    }
    if (from.has_ndestaddress()) {
      set_ndestaddress(from.ndestaddress());
    }
    if (from.has_nmsgtype()) {
      set_nmsgtype(from.nmsgtype());
    }
    if (from.has_ninfolen()) {
      set_ninfolen(from.ninfolen());
    }
    if (from.has_sinfobuff()) {
      set_sinfobuff(from.sinfobuff());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BdfsMsg::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BdfsMsg::CopyFrom(const BdfsMsg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BdfsMsg::IsInitialized() const {
  if ((_has_bits_[0] & 0x0000003f) != 0x0000003f) return false;

  return true;
}

void BdfsMsg::Swap(BdfsMsg* other) {
  if (other != this) {
    std::swap(nserialid_, other->nserialid_);
    std::swap(nsourceaddress_, other->nsourceaddress_);
    std::swap(ndestaddress_, other->ndestaddress_);
    std::swap(nmsgtype_, other->nmsgtype_);
    std::swap(ninfolen_, other->ninfolen_);
    std::swap(sinfobuff_, other->sinfobuff_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BdfsMsg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BdfsMsg_descriptor_;
  metadata.reflection = BdfsMsg_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int FsfkMsg::kNSerialIdFieldNumber;
const int FsfkMsg::kNResFieldNumber;
#endif  // !_MSC_VER

FsfkMsg::FsfkMsg()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void FsfkMsg::InitAsDefaultInstance() {
}

FsfkMsg::FsfkMsg(const FsfkMsg& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void FsfkMsg::SharedCtor() {
  _cached_size_ = 0;
  nserialid_ = GOOGLE_LONGLONG(0);
  nres_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

FsfkMsg::~FsfkMsg() {
  SharedDtor();
}

void FsfkMsg::SharedDtor() {
  if (this != default_instance_) {
  }
}

void FsfkMsg::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* FsfkMsg::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return FsfkMsg_descriptor_;
}

const FsfkMsg& FsfkMsg::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_smsTx_2eproto();
  return *default_instance_;
}

FsfkMsg* FsfkMsg::default_instance_ = NULL;

FsfkMsg* FsfkMsg::New() const {
  return new FsfkMsg;
}

void FsfkMsg::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    nserialid_ = GOOGLE_LONGLONG(0);
    nres_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool FsfkMsg::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int64 nSerialId = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &nserialid_)));
          set_has_nserialid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_nRes;
        break;
      }

      // required int32 nRes = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_nRes:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &nres_)));
          set_has_nres();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void FsfkMsg::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int64 nSerialId = 1;
  if (has_nserialid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->nserialid(), output);
  }

  // required int32 nRes = 2;
  if (has_nres()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->nres(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* FsfkMsg::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int64 nSerialId = 1;
  if (has_nserialid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->nserialid(), target);
  }

  // required int32 nRes = 2;
  if (has_nres()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->nres(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int FsfkMsg::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int64 nSerialId = 1;
    if (has_nserialid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->nserialid());
    }

    // required int32 nRes = 2;
    if (has_nres()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->nres());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void FsfkMsg::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const FsfkMsg* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const FsfkMsg*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void FsfkMsg::MergeFrom(const FsfkMsg& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_nserialid()) {
      set_nserialid(from.nserialid());
    }
    if (from.has_nres()) {
      set_nres(from.nres());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void FsfkMsg::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void FsfkMsg::CopyFrom(const FsfkMsg& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool FsfkMsg::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void FsfkMsg::Swap(FsfkMsg* other) {
  if (other != this) {
    std::swap(nserialid_, other->nserialid_);
    std::swap(nres_, other->nres_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata FsfkMsg::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = FsfkMsg_descriptor_;
  metadata.reflection = FsfkMsg_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)
