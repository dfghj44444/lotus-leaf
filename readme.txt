游戏玩法

    控制角色搭乘荷叶到达出口，从开始点选择一片荷叶，然后荷叶就会与角色一起往一个方向直线移动。游戏场景中有若干的石柱，荷叶碰到石柱后会停下并将角色放到石柱上，此时可以选择石柱周围其它的荷叶继续前进，直至到达出口。
    游戏中玩家并不能看到整个场景，在场景上有一个作为遮罩的图层，只能看到以角色为中心一定范围以内的区域。
    只有角色来到某个石柱上面时，该石柱周围的荷叶才会出现。选择一个方向的荷叶后，角色和荷叶就会一直往前移动直到碰到石柱。

游戏规则
	停留：角色站在石柱上时为停留状态，角色停留时可以选择石柱周围的荷叶来移动到其它地方。
	移动：
1.	石柱周围有四个透明按钮，当该方向上有荷叶时这个方向的透明按钮才可用，点击荷叶（透明按钮）后角色和荷叶一起往该方向直线移动。
2.	移动过程中碰到了石柱就会停下，角色移动到石柱上，荷叶停留在到达的位置。
完成：角色到达出口的石柱(五角星形)时游戏完成。



文件夹说明
/rundir 程序运行目录
    stage1.ini 关卡文件
    /img 游戏用到的图片
/lib    静态库的目录



程序编译需要DX9的SDK,VS2008

作者：邢敏
mail:dfghj77777@gmail.com
qq:343493180