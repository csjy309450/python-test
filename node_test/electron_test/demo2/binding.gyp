{
	"targets": [{
		"target_name": "modular_test",
		"include_dirs": [
			"native/"
		],
		#"libraries": [ "core_test.so" ],
		"link_settings": {
			"libraries": [ 
				"/home/yz/myPrograms/Libs_test/node_test/electron_test/demo1/native/core_test.so"
			] 
		},
		"sources": [
			"native/modular_test.cc"
		 ]
	}]
}
