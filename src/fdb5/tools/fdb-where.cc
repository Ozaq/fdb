/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/log/Log.h"
#include "eckit/option/CmdArgs.h"

#include "fdb5/api/FDB.h"
#include "fdb5/api/helpers/FDBToolRequest.h"
#include "fdb5/tools/FDBVisitTool.h"

using namespace eckit::option;
using namespace eckit;


namespace fdb5 {
namespace tools {

//----------------------------------------------------------------------------------------------------------------------

class FDBWhere : public FDBVisitTool {
  public: // methods

    FDBWhere(int argc, char **argv) :
        FDBVisitTool(argc, argv),
        porcelain_(false) {

        options_.push_back(new SimpleOption<bool>("porcelain", "Streamlined output for input into other tools"));
    }

  private: // methods

    void execute(const CmdArgs& args) override;
    void init(const CmdArgs &args) override;

  private: // members

    bool porcelain_;
};

void FDBWhere::init(const CmdArgs& args) {
    FDBVisitTool::init(args);

    args.get("porcelain", porcelain_);
}


void FDBWhere::execute(const CmdArgs&) {

    FDB fdb;

    for (const FDBToolRequest& request : requests("read")) {

        auto whereIterator = fdb.where(request);

        size_t count = 0;
        WhereElement elem;
        while (whereIterator.next(elem)) {
            Log::info() << elem << std::endl;
            count++;
        }

        if (count == 0 && fail()) {
            std::stringstream ss;
            ss << "No FDB entries found for: " << request << std::endl;
            throw FDBToolException(ss.str());
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace tools
} // namespace fdb5


int main(int argc, char **argv) {
    fdb5::tools::FDBWhere app(argc, argv);
    return app.start();
}
