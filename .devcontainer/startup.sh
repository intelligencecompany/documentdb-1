#!/bin/bash

# Define the flag file
FLAG_FILE="/home/documentdb/setup_completed"

# Check if the code directory exists
if [ ! -d "code" ]; then
    echo "Error: code directory does not exist."
    exit 1
fi

echo "Move to code dir"
cd code

# Check if the setup has already been completed
if [ ! -f "$FLAG_FILE" ]; then
    echo "First run setup"

    # Check if the Makefile exists
    if [ ! -f "Makefile" ]; then
        echo "Error: Makefile does not exist."
        exit 1
    fi

    git config --global --add safe.directory /home/DocumentDB/code
    make
    sudo make install

    ./scripts/start_oss_server.sh

    ./scripts/start_oss_server.sh -c

    # Edit the pg_hba.conf file
    echo "host    all             all             94.215.14.79/32               trust" >> ~/documentdb_test/pg_hba.conf
    echo "host    all             all             192.168.178.43/32             trust" >> ~/documentdb_test/pg_hba.conf
    echo "host    all             all             172.17.0.2/32             trust" >> ~/documentdb_test/pg_hba.conf

    # Edit the postgresql.conf file
    echo "listen_addresses = '*'" >> ~/documentdb_test/postgresql.conf

    psql -p 9712 -d postgres -c "CREATE USER app WITH SUPERUSER PASSWORD 'Apekop01!';"
    # Create the flag file to indicate setup completion
    touch "$FLAG_FILE"
else
    echo "Setup already completed, skipping setup steps"
fi

# Run the start_oss_server.sh script
./scripts/start_oss_server.sh

exec "$@"