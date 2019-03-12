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
    // https://libgit2.org/libgit2/#0.28.0/group/cred/git_cred_ssh_key_new
    return git_cred_ssh_key_new(out, "androiddev",
            "/home/alex/.ssh/id_rsa.pub", "/home/alex/.ssh/id_rsa", nullptr);
}

int main() {

    git_libgit2_init();
    auto deferred_shutdown = sl::support::defer([]() STATICLIB_NOEXCEPT {
        git_libgit2_shutdown();
    });

    git_repository* repo = nullptr;

    // flip me and provide credentials to tests ssh
    bool enable_ssh = false;
    if (enable_ssh) {
        git_clone_options opts;
        git_clone_init_options(std::addressof(opts), GIT_CLONE_OPTIONS_VERSION);
        opts.fetch_opts.callbacks.credentials = cred_cb;
        auto err = git_clone(std::addressof(repo),
                "git+ssh://androiddev@127.0.0.1/home/androiddev/app",
                "cloned_tmp_ssh", std::addressof(opts));
        slassert(0 == err);
    }

    // https
    auto err = git_clone(std::addressof(repo),
            "https://github.com/staticlibs/external_libgit2.git",
            "cloned_tmp", nullptr);
    slassert(0 == err);
    auto deferred_repo = sl::support::defer([repo]() STATICLIB_NOEXCEPT {
        git_repository_free(repo);
    });

    return 0;
}

