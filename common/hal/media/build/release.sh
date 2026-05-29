#!/bin/bash
# Copyright (c) 2025 HiSilicon (Shanghai) Technologies Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# media release script
#
set -e

chip_name=$1

DIRECTORY_TO_BE_RELEASED=(
../format/source
)

TO_BE_REMOVED=(
../format/libs/$chip_name/gcc/linux/libs/libplugin_demuxer_hmf.a
../format/libs/$chip_name/gcc/linux/libs/libplugin_demuxer_raw.a
../format/libs/$chip_name/gcc/linux/libs/libplugin_demuxer_mpf.a
../format/libs/$chip_name/gcc/linux/libs/libplugin_muxer_recorder.a
../format/libs/$chip_name/gcc/linux/libs/libplugin_muxer_m4a.a
../format/libs/$chip_name/gcc/linux/libs/libformat_hw.a

../test
../audio/libs
../codec/libs
../camera/libs
../common/libs
../format/source/include
)

# Delete all but exclude include and lib directory.
function make_publish_directory()
{
    if [ $# -lt 1 ]; then
        return
    fi

    for dir in "$@" ; do
        if [ -d "$dir" ]; then
            pushd ${dir}
	    	rm -rf $(ls |egrep -v '(include|lib)')
            popd
        fi
    done
}

function remove_dirs_and_files()
{
    if [ $# -lt 1 ]; then
        return
    fi

    for dir in "$@" ; do
        if [ -a "$dir" ]; then
            rm -rf ${dir}
        fi
    done
}

function remove_objects()
{
    find ./ -name obj | xargs rm -rf
    find ./ -name *.o | xargs rm -rf
    find ./ -name *.d | xargs rm -rf
}


function release()
{
    echo "DIRECTORY_TO_BE_RELEASED:${DIRECTORY_TO_BE_RELEASED[@]}"
    make_publish_directory ${DIRECTORY_TO_BE_RELEASED[@]}

    echo "TO_BE_REMOVED:${TO_BE_REMOVED[@]}"
    remove_dirs_and_files ${TO_BE_REMOVED[@]}

    remove_objects
}

function main(){
    module_name=$(pwd | awk -F"/" '{print $NF}')
    echo "Release module: $module_name start."
    release
    if [ "$chip_name" != "hispark_hi3516cv610" ];then
        pushd ../
        make clean
        make
        make clean
        popd
    fi
    echo "Release module: $module_name end."
}

main "$@"
