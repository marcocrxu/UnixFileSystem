#pragma once

class SuperBlock {
public:
	const static int MAX_NFREE = 100;
	const static int MAX_NINODE = 100;

public:
	int	s_isize;		        // 外存INode区占用的盘块数
	int	s_fsize;		        // 盘块总数

	int	s_nfree;		        // 直接管理的空闲盘块数量
	int	s_free[MAX_NFREE];	    // 直接管理的空闲盘块索引表

	int	s_ninode;		        // 直接管理的空闲外存INode数量
	int	s_inode[MAX_NINODE];	// 直接管理的空闲外存INode索引表

	int	s_flock;		        // 封锁空闲盘块索引表标志
	int	s_ilock;		        // 封锁空闲INode表标志

	int	s_fmod;			        // 内存中super block副本被修改标志，意味着需要更新外存对应的Super Block
	int	s_ronly;		        // 本文件系统只能读出
	int	s_time;			        // 最近一次更新时间
	int	padding[47];	        // 填充使SuperBlock块大小等于1024字节，占据2个扇区
};
