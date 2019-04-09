
microsipps.dll: dlldata.obj microsip_p.obj microsip_i.obj
	link /dll /out:microsipps.dll /def:microsipps.def /entry:DllMain dlldata.obj microsip_p.obj microsip_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del microsipps.dll
	@del microsipps.lib
	@del microsipps.exp
	@del dlldata.obj
	@del microsip_p.obj
	@del microsip_i.obj

microsipps.dll: dlldata.obj microsip_p.obj microsip_i.obj
	link /dll /out:microsipps.dll /def:microsipps.def /entry:DllMain dlldata.obj microsip_p.obj microsip_i.obj \
		kernel32.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \
.c.obj:
	cl /c /Ox /DREGISTER_PROXY_DLL \
		$<

clean:
	@del microsipps.dll
	@del microsipps.lib
	@del microsipps.exp
	@del dlldata.obj
	@del microsip_p.obj
	@del microsip_i.obj
