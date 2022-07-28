/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/*
 * @file   tests_lib.cc
 * @author Nicolau Manubens
 * @date   Jun 2022
 */

#include <unistd.h>

#include "eckit/filesystem/TmpDir.h"
#include "eckit/exception/Exceptions.h"

#include "tests_lib.h"
#include "../dummy_daos.h"

static void deldir(eckit::PathName& p) {
    if (!p.exists()) {
        return;
    }

    std::vector<eckit::PathName> files;
    std::vector<eckit::PathName> dirs;
    p.children(files, dirs);

    for (auto& f : files) {
        f.unlink();
    }
    for (auto& d : dirs) {
        deldir(d);
    }

    p.rmdir();
};

extern "C" {

int dmg_pool_create(const char *dmg_config_file,
                    uid_t uid, gid_t gid, const char *grp,
                    const d_rank_list_t *tgts,
                    daos_size_t scm_size, daos_size_t nvme_size,
                    daos_prop_t *prop,
                    d_rank_list_t *svc, uuid_t uuid) {

    std::string pool_name;

    if (prop != NULL) {

        if (prop->dpp_nr != 1) NOTIMP;
        if (prop->dpp_entries[0].dpe_type != DAOS_PROP_PO_LABEL) NOTIMP;

        struct daos_prop_entry *entry = &prop->dpp_entries[0];

        if (entry == NULL) NOTIMP;

        pool_name = std::string(entry->dpe_str);

    }

    std::string random_name;

    random_name = eckit::TmpDir().baseName().path();
    random_name += "_" + std::to_string(getpid());

    const char *random_name_cstr = random_name.c_str();

    uuid_t seed = {0};

    uuid_generate_md5(uuid, seed, random_name_cstr, strlen(random_name_cstr));

    char pool_uuid_cstr[37] = "";
    uuid_unparse(uuid, pool_uuid_cstr);

    eckit::PathName pool_path = dummy_daos_root() / pool_uuid_cstr;

    if (pool_path.exists()) return -1;

    pool_path.mkdir();

    if (prop != NULL) {

        eckit::PathName label_symlink_path = dummy_daos_root() / pool_name;

        ::symlink(pool_path.path().c_str(), label_symlink_path.path().c_str());

    }

    return 0;

}

int dmg_pool_destroy(const char *dmg_config_file,
                     const uuid_t uuid, const char *grp, int force) {
    
    char uuid_str[37] = "";

    uuid_unparse(uuid, uuid_str);

    eckit::PathName pool_path = dummy_daos_root() / uuid_str;

    if (!pool_path.exists()) return -1;

    std::vector<eckit::PathName> files;
    std::vector<eckit::PathName> dirs;
    dummy_daos_root().children(files, dirs);

    for (auto& f : files) {
        if (f.isLink() && f.realName() == pool_path) {
            f.unlink();
        }
    }

    deldir(pool_path);

    return 0;

}

}  // extern "C"
