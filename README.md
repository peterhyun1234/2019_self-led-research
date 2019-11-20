# 2019_self-led-research
Optimization of communication layer for distributed block storage
------------------------------------
## 0. Research background

## 1. Block unit storage
---------------------------------------- 
## 2. epoll (multiplexing)
![web](https://user-images.githubusercontent.com/46476398/68107383-0a69ee80-ff28-11e9-9e6a-879444c84eb8.png)
---------------------------------------- 
## 3. System structure (multithread <-> epoll)
### 3.1. Servers (epoll)
 $ sudo apt-get update 
    $ sudo apt-get install nodejs
    $ sudo apt-get install npm
   
()

### 3.2. Clients (multi-thread)
()
----------------------------------------    
#### 4.2.1. SQL with added ability to sort by search goodness   
```java
SELECT keywords,
  (
    ((LENGTH(keywords) - LENGTH((REPLACE(keywords, '이효리', '')))) / LENGTH('이효리'))
    + ((LENGTH(keywords) - LENGTH((REPLACE(keywords, '한예슬', '')))) / LENGTH('한예슬'))
    + ((LENGTH(keywords) - LENGTH((REPLACE(keywords, '전지현', '')))) / LENGTH('전지현'))
  ) AS score
FROM sentence
WHERE keywords LIKE '%이효리%'
  OR keywords LIKE '%한예슬%'
  OR keywords LIKE '%전지현%'
ORDER BY score DESC
```
> 중복된 단어를 제외한 단어 적합도(카운트)에 따라 정렬하는 쿼리  
----------------------------------------
