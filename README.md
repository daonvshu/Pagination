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
