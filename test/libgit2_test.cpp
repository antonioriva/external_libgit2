/*
 * Copyright 2019, alex at staticlibs.net
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   libgit2_test.cpp
 * Author: alex
 *
 * Created on March 10, 2019, 10:55 AM
 */

#include "git2.h"

#include <iostream>

#include "staticlib/config/assert.hpp"
#include "staticlib/support.hpp"

int cred_cb(git_cred** out, const char*, const char*, unsigned int, void*) {
    // https://libgit2.github.com/libgit2/#HEAD/group/cred/git_cred_ssh_key_new
    return git_cred_ssh_key_new(out, "androiddev",
            "/home/alex/.ssh/id_rsa.pub", "/home/alex/.ssh/id_rsa", nullptr);
}

int main() {

    git_libgit2_init();
    auto deferred_shutdown = sl::support::defer([]() STATICLIB_NOEXCEPT {
        git_libgit2_shutdown();
    });

    // flip this flag to enable clone
    bool enabled = false;
    if (enabled) {
        git_repository* repo = nullptr;

        git_clone_options opts;
        git_clone_init_options(std::addressof(opts), GIT_CLONE_OPTIONS_VERSION);
        opts.fetch_opts.callbacks.credentials = cred_cb;
        
        auto err = git_clone(std::addressof(repo),
                "https://github.com/staticlibs/external_libgit2.git",
                //"git+ssh://androiddev@127.0.0.1/home/androiddev/app",
                "lookaside_libgit2_tmp", std::addressof(opts));
//        std::cout << giterr_last()->message << std::endl;
        slassert(0 == err);
        auto deferred_repo = sl::support::defer([repo]() STATICLIB_NOEXCEPT {
            git_repository_free(repo);
        });
    }

    return 0;
}

