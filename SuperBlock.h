#pragma once

class SuperBlock {
public:
	const static int MAX_NFREE = 100;
	const static int MAX_NINODE = 100;

public:
	int	s_isize;		        // ���INode��ռ�õ��̿���
	int	s_fsize;		        // �̿�����

	int	s_nfree;		        // ֱ�ӹ���Ŀ����̿�����
	int	s_free[MAX_NFREE];	    // ֱ�ӹ���Ŀ����̿�������

	int	s_ninode;		        // ֱ�ӹ���Ŀ������INode����
	int	s_inode[MAX_NINODE];	// ֱ�ӹ���Ŀ������INode������

	int	s_flock;		        // ���������̿��������־
	int	s_ilock;		        // ��������INode���־

	int	s_fmod;			        // �ڴ���super block�������޸ı�־����ζ����Ҫ��������Ӧ��Super Block
	int	s_ronly;		        // ���ļ�ϵͳֻ�ܶ���
	int	s_time;			        // ���һ�θ���ʱ��
	int	padding[47];	        // ���ʹSuperBlock���С����1024�ֽڣ�ռ��2������
};
