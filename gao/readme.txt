文件改名
ls| awk '{printf("mv %s %s\n", $0, tolower($0))|"sh"}' 
