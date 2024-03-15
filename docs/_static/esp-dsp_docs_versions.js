var DOCUMENTATION_VERSIONS = {
    DEFAULTS: { has_targets: false,
                supported_targets: [ "esp32" ]
              },
    VERSIONS: [
        { name: "latest" },
        { name: "v1.4.2", old:false },
        { name: "v1.2.1", old:true },
        { name: "v1.1.0", old:true }
    ],
    IDF_TARGETS: [
       { text: "all targets", value: "esp32"},
    ]
};
