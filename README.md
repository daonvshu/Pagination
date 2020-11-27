# Pagination
 a paging widget based on Qt

### Usage Demo
``` c++
auto widget = new QWidget(this);
widget->setGeometry(0, 0, 560, 50);

auto hbox = new QHBoxLayout(widget);
hbox->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

auto pagination = new Pagination(widget);
hbox->addWidget(pagination);

auto text = new QLabel(this);
text->setGeometry(120, 50, 40, 20);

connect(pagination, &Pagination::selectPage, [=](int value) {
	text->setText(QString::number(value));
});

pagination->setTotalSize(20);
```

![image](https://github.com/daonvshu/Pagination/blob/master/demo.png?raw=true)

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
![image](https://github.com/daonvshu/Pagination/blob/master/demo2.png?raw=true)