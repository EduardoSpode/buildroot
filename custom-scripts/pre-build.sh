#!/bin/sh

cp $BASE_DIR/../custom-scripts/S41network-config $BASE_DIR/target/etc/init.d
cp $BASE_DIR/../custom-scripts/script_time.sh $BASE_DIR/target/bin/script_time.sh
chmod +x $BASE_DIR/target/etc/init.d/S41network-config
make -C $BASE_DIR/../modules/simple_driver/
make -C $BASE_DIR/../modules/hello/
make -C $BASE_DIR/../disk-test/
make -C $BASE_DIR/../modules/sstf-iosched/
