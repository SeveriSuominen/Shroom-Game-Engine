#pragma once
//------------------------------------------
// JSON
//------------------------------------------
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/istreamwrapper.h>
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

#include <fstream>
#include <tuple>

using namespace rapidjson;

//TYPE DEFS 
typedef Writer<StringBuffer> SHROOM_JSON_WRITER;
typedef Document SHROOM_JSON_DOCUMENT;
typedef rapidjson::Value& SHROOM_JSON_DOCUMENT_ENTRY;
//------------------------------------------

#define THROW_INVALID_DESERILIZATION_TARGET() std::stringstream ss; ss << "Invalid deserializer target: " << __LINE__ << __FILE__; throw std::exception(ss.str().c_str());