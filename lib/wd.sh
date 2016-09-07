#!/usr/bin/sh
#
# ▄▄▌ ▐ ▄▌·▄▄▄▄
# ██· █▌▐███▪ ██
# ██▪▐█▐▐▌▐█· ▐█▌
# ▐█▌██▐█▌██. ██
# ▀▀▀▀ ▀▪▀▀▀▀▀•
#
# wrapper for: sh
#
# @github.com/mfaerevaag/wd-c

wd() {
    output=$(_wd $@)
    ret=$?

    if [[ $ret -eq 0  ]]
    then
        cd "$output"
    else
        if [[ "$output" != "" ]]
        then
            echo "$output"
        fi
    fi

    unset output
    unset ret
}
