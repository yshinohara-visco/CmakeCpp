# CmakeCpp

## �T�v

Cmake �v���W�F�N�g�ō쐬���� DLL �����S�ɕʂ̃v���W�F�N�g����g�p����ɂ͉����v�邩���m�F�����B

## �ڍ�

LibProject �ŐÓI���C�u���� ModuleStatic �Ɠ��I���C�u���� ModuleShared ���쐬���Ă���B
LibPriject �����炱�����g�p����ɂ� add_subdirectory ����� target_link_directories �ł����B

�S���ʂ� Cmake �v���W�F�N�g ExeProject ����g�p����ɂ͍Œ�������K�v���H
cmake �œK�؂ɃC���X�g�[��������o�͂�����@���ǂ�������Ȃ������̂ŁA���ƂŃR�s�[���Ȃ��玎�����B

~~�܂��A�R���p�C���� ModuleStaic.h �� ModuleShared.h �̗������K�v�������B~~
~~����� ModuleShared.h �� ModuleStaic.h ���C���N���[�h���邩��B~~

~~�����N�ɂ� ModuleStatic.lib �� ModuleShared.lib �̗������K�v�������BModuleShared.lib �� Static �̓��e���܂܂��Ƃ������Ƃ͖����炵���B~~

���s�ɂ� ModuleShared.dll ���K�v�������B

~~�ꕔ�� DLL �ɂ��Č��J�������������A���ǊJ���ɂ͑S�Ẵw�b�_�Alib ���K�v�ƍl���������ǂ������B~~

�F�X���Ⴂ���Ă����̂ōČ��؂���
Static.lib ���K�v�ɂȂ��Ă����̂͂��̂悤�ɏ����Ă������炾�����B
1�DModuleShared.h �� ModuleStatic.h ���C���N���[�h���Ȃ���΁AExeProject �ɂ� ModuleShared.h ��������΂���
2�D���̏�� CmakeLists.txt �� ModuleStatic �������Ȃ���΁A�r���h�� ModuleStatic.lib �͕K�v�Ȃ�
3�D���s�ɂ� ModuleShared.dll ���K�v

�w�b�_���� Static �̗v�f���B���΁AShared �������Q�Ƃ��� Static ���C�u�����͗��p�҂��猩����K�v�͖����B
