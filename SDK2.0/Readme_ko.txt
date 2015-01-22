
1. adStar SDK 를 처음 다운로드 받았다면 먼저 lib/adstar.epx 를 EISC Studio 3 로 오픈 한 다음
   Build/Build Project 를 실행 하여 lib/libadstar.a 파일을 생성 해야 한다.
   example 폴더내의 모든 예제들은 lib/libadstar.a 파일을 사용하게 되어 있다.

2. STK Version 2.0 이상의 보드를 사용할 경우 /include/stk_board.h에서 #define STK_20을 활성화 시키고 compile해야 한다.
    Version 2.0 미만 보드에서는 #define STK_20을 주석 처리하고 사용해야 한다.