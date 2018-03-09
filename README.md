wd
==

`wd` (*warp directory*) lets you jump to custom directories in your terminal, without using `cd`. Why? Because `cd` seems inefficient when the folder is frequently visited or has a long path.

*NOTE*: This is a port written in `c`, which can be used in all shells using wrappers. If you're using [oh-my-zsh](https://github.com/robbyrussell/oh-my-zsh), check out the [original wd](https://github.com/mfaerevaag/wd).


# Install

To install properly you need to

1. compile and install binary (`/usr/bin/_wd`)
2. copy and source the wanted wrapper function (`wd`)


## 1. Binary (`_wd`)

### Manual

Checkout / download latest release

    # make
    # make install

### Arch ([AUR](https://aur.archlinux.org/))

    # yaourt -S wd-git

## 2. Wrapper (`wd`)

Link (or copy) wrapper of your desired shell somewhere convenient. There are wrappers for `sh`, `zsh` and `fish`, but the `sh` should conquer all, no matter shell.

Example for `bash` using the `.sh` wrapper:

    # ln -s /usr/share/wd/wd.sh ~/bin/wd.sh
    # echo "source ~/bin/wd.sh" >> ~/.bashrc


# Usage

See original [README](https://github.com/mfaerevaag/wd).

Note that argument syntax has changed to standard `getopt_long` (prefixed with single og double dash, e.g. `-a` / `--add`), and that some functions are _not yet implemented_ (see list below).


# Contribution

Use the test suite with

    # make debug
    # make test

Not yet implemented:

 - [X] Warp directly to subdirectory of point
 - [ ] Overwriting (force flag)
 - [ ] Clean rc-file
 - [ ] Tab completion (bash is implemented)
