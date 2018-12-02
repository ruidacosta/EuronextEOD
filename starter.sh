#!/bin/sh

envsubst < config_template.ini > config.ini
/usr/bin/python ./FileDownloader -c config.ini && ./euronextEOD config.ini
