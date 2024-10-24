#!/bin/bash
scp src/cat/s21_cat cd@10.0.2.14:/usr/local/bin/
scp src/grep/s21_grep cd@10.0.2.14:/usr/local/bin/
ssh cd@10.0.2.14 ls /usr/local/bin

