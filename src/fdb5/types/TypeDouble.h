/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file   TypeDouble.h
/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date   April 2016

#ifndef fdb5_TypeDouble_H
#define fdb5_TypeDouble_H

#include "fdb5/types/Type.h"

namespace fdb5 {

//----------------------------------------------------------------------------------------------------------------------

class TypeDouble : public Type {

public: // methods

    TypeDouble(const std::string &name, const std::string &type);

    virtual ~TypeDouble() override;

    virtual std::string toKey(const std::string& value) const override;

    virtual void getValues(const metkit::mars::MarsRequest &request,
                           const std::string &keyword,
                           eckit::StringList &values,
                           const Notifier &wind,
                           const DB *db) const override;

private: // methods

    virtual void print( std::ostream &out ) const override;

};

//----------------------------------------------------------------------------------------------------------------------

} // namespace fdb5

#endif
