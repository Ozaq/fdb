/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Simon Smart
/// @date   November 2018

#ifndef fdb5_api_WipeIterator_H
#define fdb5_api_WipeIterator_H

#include "fdb5/api/helpers/APIIterator.h"

#include "eckit/filesystem/PathName.h"

#include <string>

/*
 * Define a standard object which can be used to iterate the results of a
 * wipe() call on an arbitrary FDB object
 */

namespace fdb5 {

class IndexLocation;

//----------------------------------------------------------------------------------------------------------------------

// TODO: We might expand this to include host, port, etc, in an explicit where object.
//       Or to use eckit::URL, ...

class WipeElement {

public: // methods

    WipeElement() = default;
    WipeElement(eckit::Stream& s);

    size_t guessEncodedSize() const;

private: // methods

    void encode(eckit::Stream& s) const;

    friend eckit::Stream& operator<<(eckit::Stream& s, const WipeElement& r) {
        r.encode(s);
        return s;
    }

public: // members

    std::string owner;
    std::set<eckit::PathName> metadataPaths;
    std::set<eckit::PathName> dataPaths;
    std::set<eckit::PathName> otherPaths;
    std::set<eckit::PathName> safePaths;
    std::vector<std::shared_ptr<const IndexLocation>> indexes;
};

using WipeIterator = APIIterator<WipeElement>;

using WipeAggregateIterator = APIAggregateIterator<WipeElement>;

using WipeAsyncIterator = APIAsyncIterator<WipeElement>;

//----------------------------------------------------------------------------------------------------------------------

} // namespace fdb5

#endif
