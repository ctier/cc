@echo ��ʼ�����Ŀ¼
if exist output rd /q /s output

if not exist output\sdk\inc md output\sdk\inc
if not exist output\sdk\lib md output\sdk\lib
if not exist output\bin md output\bin

set LIBINIT="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32.bat"
call %LIBINIT%

@echo "��ʼ����PhoneLib����"
devenv "PhoneLib\PhoneLib.vcproj" /rebuild Debug /project PhoneLib
devenv "PhoneLib\PhoneLib.vcproj" /rebuild Release /project PhoneLib


@echo "��ʼѹ��sip��ؿ⡭��"
lib ./PhoneLib/Debug/PhoneLib.lib ./pjsipLib/*-i386-win32-vc6-debug.lib /out:./output/sdk/lib/PhoneLib-Debug.lib
lib ./PhoneLib/Release/PhoneLib.lib ./pjsipLib/*-i386-win32-vc6-release.lib /out:./output/sdk/lib/PhoneLib-Release.lib


@echo "��ʼ������Գ��򡭡�"
devenv "SIPPhoneTest\SIPPhone.vcproj" /rebuild Debug /project SIPPhone
devenv "SIPPhoneTest\SIPPhone.vcproj" /rebuild Release /project SIPPhone

