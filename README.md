# Pagination
 a paging widget based on Qt

## stylesheet demo
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

## paging style
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

## function
> properties

|name|description|getter/setter|
|----|----|----|
|`spacing`|spacing between buttons|getBoxSpacing/setBoxSpacing|
|`totalsize`|all of display item size|getTotalSize/getTotalSize|
|`sizeofperpage`|size of one page|getSizeofPerPage/setSizeofPerPage|
|`pageUpText`|left page button display text|getPageUpText/setPageUpText|
|`pageDownText`|right page button display text|getPageDownText/setPageDownText|

build the plugin(`pagination_plugin.dll`) project, and use `pagination` widget in `Qt Designer`  
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/designer.drawio.png?raw=true)  

configure properties in `Qt Designer` property window  
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/designer-property.png?raw=true)  

what's means of this properties:  
![image](https://github.com/daonvshu/Pagination/blob/master/screenshot/display.drawio.png?raw=true)  

> public functions
- `void setPagingStyle(PagingUtil* pagingStyle)`  
  Change the current page numbers display style.  
  By default, use `PagingStyle1`.  
  The ownership of `pagingStyle` is transferred to the pagination widget, and it's the widget's responsibility to delete it.
- `void setCurrentPage(int number)`  
  Change the current selected number, the `number` is used to display instead of index. If the `number` is different from the current number, `pageIndexChanged` will be emit.  
- `void pageUp()`  
  Jump to previous page.
- `void pageDown()`  
  Jump to next page.
- `void pageFirst()`  
  Jump to the first page.
- `void pageLast()`  
  Jump to last page.
- `int getCurrentPageIndex() const`  
  Return current selected page index.

> signals
- `void pageIndexChanged(int index)`  
  will be emit whenever the number changes.  

> protected
- `virtual QString numberFormat(int number) const`  
  Can be implemented in subclasses to reformat current display number to string, and -1 if it's an ellipsis.  
- `virtual void drawNumber(const QRect& rect, int number, QPainter* painter)`  
  Draw the number in target rect, can paint your own number display style in subclasses.  
  For the hover state, to check `'hoverNumber == number'`.  
  For the selected state, to check `'pagingStyle->isSelectedNumber(number)'`.
- `virtual void drawEllipsis(const QRect& rect, int number, QPainter* painter)`  
  Draw an ellipsis only.
- `virtual void drawPageUpBtn(const QRect& rect, QPainter* painter)`  
  Draw previous page button.  
  For the hover state, to check `'hoverNumber == -3'`. 
- `virtual void drawPageDownBtn(const QRect& rect, QPainter* painter)`  
  Draw next page button.  
  For the hover state, to check `'hoverNumber == -2'`. 

## qml
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