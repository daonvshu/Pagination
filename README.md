# Pagination
 a paging widget based on Qt

### Usage
```
auto pagination = new Pagination(parent);
auto value = new QLabel(parent);
connect(pagination, &Pagination::selectPage, [=](int value) {
		value->setText(QString::number(value));
});
pagination->setTotalSize(20);
```
