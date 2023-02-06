/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/exception/Exceptions.h"
#include "eckit/utils/Tokenizer.h"
#include "eckit/filesystem/PathName.h"

#include "fdb5/daos/DaosName.h"
#include "fdb5/daos/DaosSession.h"
#include "fdb5/daos/DaosPool.h"
#include "fdb5/daos/DaosContainer.h"
#include "fdb5/daos/DaosHandle.h"
#include "fdb5/daos/DaosException.h"

namespace fdb5 {

//----------------------------------------------------------------------------------------------------------------------

DaosName::DaosName(const std::string& pool, const std::string& cont, const fdb5::DaosOID& oid) : pool_(pool), cont_(cont), oid_(oid) {

    ASSERT(oid_.wasGenerated());

}

DaosName::DaosName(const std::string& name) {

    /// @todo: find format for documenting inputs
    // Inputs:
    //   name: [pool/]cont[/oidhi.oidlo]

    eckit::Tokenizer parse("/");
    std::vector<std::string> bits;
    parse(name, bits);

    ASSERT(bits.size() == 3);

    pool_ = bits[0];
    cont_ = bits[1];
    oid_ = DaosOID(bits[2]);

}

DaosName::DaosName(const eckit::URI& uri) : DaosName(uri.name()) {}

DaosName::DaosName(const fdb5::DaosObject& obj) : 
    pool_(obj.getContainer().getPool().name()), 
    cont_(obj.getContainer().name()), 
    oid_(obj.OID()) {}

daos_size_t DaosName::size() {

    fdb5::DaosSession s{};
    // TODO: check oid.otype and build accordingly
    fdb5::DaosArray obj(s, *this);
    return obj.size();

}

bool DaosName::exists() {

    fdb5::DaosSession s{};
    try {
        // TODO: check oid.otype and build accordingly
        fdb5::DaosArray obj(s, *this);
    } catch (const fdb5::DaosEntityNotFoundException& e) {
        return false;
    }
    return true;

}

std::string DaosName::asString() const {

    return pool_ + '/' + cont_ + '/' + oid_.asString();

}

eckit::URI DaosName::URI() const {

    return eckit::URI("daos", eckit::PathName(asString()));

}

std::string DaosName::poolName() const {

    return pool_;
    
}

std::string DaosName::contName() const {

    return cont_;
    
}

fdb5::DaosOID DaosName::OID() const {

    return oid_;
    
}

eckit::DataHandle* DaosName::dataHandle(bool overwrite) const {

    /// @todo: OK to serialise pointers in DaosName?

    return new fdb5::DaosHandle(*this);

}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace fdb5