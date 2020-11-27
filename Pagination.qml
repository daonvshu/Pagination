import QtQuick 2.15

Row {
    id: root

    spacing: 4

    property var pageSize: 1
    property var currentValue: 1

    function refreshBtn() {
        var model = []
        if (pageSize < 10) { //小于10直接全部显示
            for (var i=0; i<pageSize; i++){
                model.push({number: i+1, selected: currentValue === i + 1}) //number：显示值，selected：是否选中
            }
        } else {
            //插入1,2
            [1, 2].map(v => model.push({number: v, selected: currentValue === v}));
            //判断第3个位置是否是省略号，number为-1表示省略号
            model.push({number: currentValue - 2 > 1 + 3 ? -1 : 1 + 2, selected: currentValue === 3});
            //向currentValue两边分别添加2个值
            for (var k = Math.max(1 + 3, currentValue - 2); k <= Math.min(pageSize - 3, currentValue + 2); k++) {
                model.push({number: k, selected: currentValue === k});
            }
            //判断第pageSize-2位置是否是省略号
            model.push({number: currentValue + 2 < pageSize - 3 ? -1 : pageSize - 2, selected: currentValue === pageSize - 2});
            //插入pageSize-1,pageSize
            [pageSize - 1, pageSize].map(v => model.push({number: v, selected: currentValue === v}));
        }
        btnGroup.model = model
    }

    onPageSizeChanged: {
        currentValue = 1
        if (pageSize < 1) {
            pageSize = 1
        }
        refreshBtn()
    }

    StateBtn {
        text: qsTr("上一页")
        padding: 16

        onBtnSelected: {
            if (currentValue - 1 >= 1) {
                currentValue--
                refreshBtn()
            }
        }
    }

    Repeater {
        id: btnGroup
        model: [{number: 1, selected: true}]
        delegate: StateBtn {
            width: 24
            text: modelData.number === -1 ? "..." : ("" + modelData.number)
            selectable: true
            selected: modelData.selected
            enabled: modelData.number !== -1 //禁用省略号

            onBtnSelected: {
                currentValue = btnGroup.model[index].number
                refreshBtn()
            }
        }
    }

    StateBtn {
        text: qsTr("下一页")
        padding: 16

        onBtnSelected: {
            if (currentValue + 1 <= pageSize) {
                currentValue++
                refreshBtn()
            }
        }
    }
}
