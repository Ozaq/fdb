/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file   MultiRetrieveVisitor.h
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date   October 2016

#ifndef fdb5_MultiRetrieveVisitor_H
#define fdb5_MultiRetrieveVisitor_H

#include <string>

#include "fdb5/database/ReadVisitor.h"
#include "fdb5/config/Config.h"

#include "eckit/container/CacheLRU.h"

namespace fdb5 {

class HandleGatherer;
class Notifier;

class DB;

//----------------------------------------------------------------------------------------------------------------------

class MultiRetrieveVisitor : public ReadVisitor {

public: // methods

    MultiRetrieveVisitor(const Notifier& wind,
                         HandleGatherer& gatherer,
                         eckit::CacheLRU<Key,DB*>& databases,
                         const Config& config);

    ~MultiRetrieveVisitor();

private:  // methods

    // From Visitor

    virtual bool selectDatabase(const Key &key, const Key &full);

    virtual bool selectIndex(const Key &key, const Key &full);

    virtual bool selectDatum(const Key &key, const Key &full);

    virtual void values(const MarsRequest& request,
                        const std::string& keyword,
                        const TypesRegistry& registry,
                        eckit::StringList& values);

    virtual void print( std::ostream &out ) const;

private:

    DB* db_;

    const Notifier& wind_;

    eckit::CacheLRU< Key,DB*>& databases_;

    HandleGatherer &gatherer_;

    Config config_;
};

//----------------------------------------------------------------------------------------------------------------------

} // namespace fdb5

#endif
