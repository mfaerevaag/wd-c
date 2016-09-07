#!/usr/bin/fish
#
# ▄▄▌ ▐ ▄▌·▄▄▄▄
# ██· █▌▐███▪ ██
# ██▪▐█▐▐▌▐█· ▐█▌
# ▐█▌██▐█▌██. ██
# ▀▀▀▀ ▀▪▀▀▀▀▀•
#
# wrapper for: fish
#
# place in:
# ~/.config/fish/functions/wd.fish
#
# @github.com/mfaerevaag/wd-c

function wd -d 'warp directory'
    if set output (_wd $argv)
        cd $output
    else
        for line in $output
            echo $line
        end
    end
end
