## Description
Kelly Simulation in python.
To run it,
1. Clone the project.
2. Make virtual environments (optional)
3. Install Prerequisites
4. Compile and run

##  1. Cloning Project Source
```
git clone https://gitlab.com/qraftec-dev4/Kelly_matlab2python.git
cd Kelly_matlab2python
```

##  2. Use virtualenv (optional)
For windows cmd,
```
pip install virtualenv
virtualenv .
./Scripts/activate
```

For linux bash,
```
pip install virtualenv
virtualenv .
source bin/activate
```

## 3. Install Prerequisites
```
pip install -r requirements.txt
```

## 4. Compile and Run
```
python -m py_compile .\kelly_engine.py
./__pycache_
 .\__pycache__\kelly_engine.cpython-36.pyc
```

## 5. Overview
```

Kelly Ratio 기준 최적의 포트폴리오 생성해주는 유틸리티입니다.
Input Data로 펀드에 대한 정보를 담은 csv파일과 이 펀드의 가격 데이터 csv 파일이 필요 합니다.

config.pcfg 파일에서 사용자는 세부 옵션을 조정할 수 있습니다.
'''
## 6. Procedure 
'''
각 종목에 대한 가격 데이터를 전일 등락율 데이터로 변환합니다.
이 가격 데이터를 통해 Asset Group 마다 최적의 Kelly ratio를 얻습니다.
그 후 여러 종목들을 조합해 Rebalance date마다 최적의 포트폴리오를 찾습니다.

이 포트폴리오는 임의의 사용자 정의 함수로 Simulation 할 수 있습니다.
Kelly Engine은 사용자가 노이즈를 섞은 포트폴리오 데이터에 대해서 다시 제한된 조건을 만족하는
포트폴리오를 구합니다.
'''
## 7. Documents 
'''
Config 파일은 다음을 모두 포함해야 합니다.

path -
 1. fund_list : 펀드 정보를 가진 CSV파일 디렉토리
 2. price_data : 펀드 가격정보를 가진 CSV파일 디렉토리
 3. save : 결과값이 저장될 디렉토리

rebalanceT : 리밸런스 주기를 설정합니다. rebalanceT , rebalance_day_in_month 둘 중 하나의 값만 지정해도 무방합니다.
rebalance_day_in_month : 리밸런스를 월단위로 하고 싶을때 지정합니다. 만약 1로 설정한다면, 매월 1일에 리밸런싱합니다.
num_funds : 선정할 펀드 종목으 개수 입니다.
save_tables : 결과를 csv파일로 저장여부 옵션입니다.

Etc =======
sutil에 관한 내용은 kelly_lib.docx를 참고해 주시기 바랍니다.


```
