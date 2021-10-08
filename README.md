# Pagination
 a paging widget based on Qt

### stylesheet demo
```
Pagination{
	font-size:16px;
	font-family:"Microsoft YaHei UI";
	padding:8px;

	qproperty-spacing: 6;
	qproperty-totalsize: 46;
	qproperty-sizeofperpage: 2;
}

Pagination>QPushButton{
	border-radius:4px;
	color:#666;
	background:white;
}

Pagination>QPushButton:disabled{
	color:#666;
	background:transparent;
}

Pagination>QPushButton:hover{
	color:white;
	background:#86EDD9;
}

Pagination>QPushButton:selected{
	color:white;
	background:#1ABC9C;
}

Pagination>QPushButton:pressed{
	color:white;
	background:#1ABC9C;
}
```
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/p1.gif?raw=true)

```
Pagination{
	font-size:16px;
	font-family:"Microsoft YaHei UI";
	padding:8px;

	qproperty-spacing: 6;
	qproperty-totalsize: 46;
	qproperty-sizeofperpage: 2;
	qproperty-pageUpText: "上一页";
	qproperty-pageDownText: "下一页";
}
Pagination>QPushButton{
	border:1px solid #dce0e0;
	color:#666;
	background:white;
}
Pagination>QPushButton:selected{
	color:white;
	background:#0073A9;
}
Pagination>QPushButton:pressed{
	color:white;
	background:#0073A9;
}
```
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/p2.gif?raw=true)

```
Pagination{
	font-size:16px;
	font-family:"Microsoft YaHei UI";
	padding:8px;

	qproperty-spacing: 1;
	qproperty-totalsize: 46;
	qproperty-sizeofperpage: 2;
	qproperty-pageUpText: "上一页";
	qproperty-pageDownText: "下一页";
}
Pagination>QPushButton{
	color:#666;
	background:white;
	border:none;
}
Pagination>QPushButton:hover{
	color:#0073A9;
}
Pagination>QPushButton:selected{
	color:white;
	background:#0073A9;
}
Pagination>QPushButton:pressed{
	color:white;
	background:#0073A9;
}

/*your parent widget stylesheet:*/
background:#dce0e0;
```
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/p3.gif?raw=true)

### paging style
```cpp
pagination->setPagingStyle(new PagingStyle2);
```
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/p4.gif?raw=true)
  
customize your own paging style
```cpp
#include "pagingutil.h"

class MyPagingStyle : public PagingUtil {
public:
	using PagingUtil::PagingUtil;

	void reCacheNumbers() override {
		curCacheNumbers.clear();
		//do your own paging cache here
	}
}

//then..
pagination->setPagingStyle(new MyPagingStyle);
```
Note: The `curCacheNumbers` stored numbers for display, or -1 if it's an ellipsis

### qml
```
Rectangle {
	width: 600
	height: 60
	color: "#303030"

	Pagination {
		id: pagination
		anchors.right: parent.right
		anchors.rightMargin: 8
		anchors.verticalCenter: parent.verticalCenter
		pageSize: 21

		onCurrentValueChanged: {
			console.log(pagination.currentValue)
		}
	}
}
```
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/demo2.png?raw=true)