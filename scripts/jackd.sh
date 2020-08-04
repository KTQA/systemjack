#!/usr/bin/env bash

# set directories for scripts.
INI_FILES=${INI:-"/etc/systemjack"}
SCRIPT_DIR=${SCRIPTS:-"/usr/lib/systemjack"}

# load functions and environment
. "${SCRIPT_DIR}/functions.sh"
. "${INI_FILES}/env.sh"

# and, go!


eval `$READINI -i "${INI_FILES}/main.ini" jack:options=options`
say "starting: '/usr/bin/jackd ${options}'"
exec /usr/bin/jackd ${options}
