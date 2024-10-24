#!/bin/bash
URL="https://api.telegram.org/bot7064931065:AAEy4Q4rDu7UOLTHOhzwdZ88oBLVAnSs5lI/sendMessage"
TEXT=" $1%0A%0AProject: $CI_PROJECT_NAME%0AStatus: $CI_JOB_STATUS%0AStage: $CI_JOB_NAME%0AURL: $CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch: $CI_COMMIT_REF_SLUG"
curl -d "chat_id=601999627&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null