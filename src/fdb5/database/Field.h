/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Baudouin Raoult
/// @author Tiago Quintino
/// @date Sep 2012

#ifndef fdb5_Field_H
#define fdb5_Field_H

#include <chrono>

#include "eckit/eckit.h"

#include "eckit/filesystem/PathName.h"
#include "eckit/io/Length.h"
#include "eckit/io/Offset.h"
#include "eckit/memory/NonCopyable.h"
#include "eckit/types/Types.h"
#include "eckit/types/FixedString.h"

#include "fdb5/database/IndexAxis.h"
#include "fdb5/database/Key.h"
#include "fdb5/database/FieldLocation.h"
#include "fdb5/database/FieldDetails.h"

namespace eckit {
class DataHandle;
}

namespace fdb5 {

//----------------------------------------------------------------------------------------------------------------------

class Field {

public: // methods

    Field();

    Field(const FieldLocation& location, std::chrono::system_clock::time_point timestamp, const FieldDetails& details = FieldDetails());

    eckit::DataHandle* dataHandle() const { return location_->dataHandle(); }
    eckit::DataHandle* dataHandle(const Key& remapKey) const { return location_->dataHandle(remapKey); }

    const FieldLocation& location() const { return *location_; }

    std::chrono::system_clock::time_point timestamp() const { return timestamp_; }

    /// stableLocation is an object with validity that extends longer than that of the
    /// owning DB. May need converting to a more static form --- or not.
    std::shared_ptr<const FieldLocation> stableLocation() const {
        return std::const_pointer_cast<const FieldLocation>(location_->stableLocation());
    }
    const FieldDetails& details() const { return details_; }

private: // members

    std::shared_ptr<FieldLocation> location_;

    std::chrono::system_clock::time_point timestamp_;

    FieldDetails details_;

    void print( std::ostream &out ) const;

    friend std::ostream &operator<<(std::ostream &s, const Field &x) {
        x.print(s);
        return s;
    }

};

//----------------------------------------------------------------------------------------------------------------------

} // namespace fdb5

#endif
