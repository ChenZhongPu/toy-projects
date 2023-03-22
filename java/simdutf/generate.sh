#!/bin/bash

mkdir data >/dev/null 2>&1

if [[ ! -f "data/financial.csv" ]]
then
  echo "downloading financial.csv (ASCII)"
  curl -o data/financial.csv  https://www.stats.govt.nz/assets/Uploads/Annual-enterprise-survey/Annual-enterprise-survey-2021-financial-year-provisional/Download-data/annual-enterprise-survey-2021-financial-year-provisional-csv.csv
fi

if [[ ! -f "data/uuid.txt" ]]
then
  echo "generating uuid.txt (ASCII)"
  for _ in {1..20000}
  do
    openssl rand -base64 200 >> data/uuid.txt
  done
fi