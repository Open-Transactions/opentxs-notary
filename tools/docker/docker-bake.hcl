variable "tag" { default = "23.2.1" }
variable "opentxs" { default = "1.189.0" }

variable "target_arch" {
  default = "amd64"
}
variable "repo" {
  default = "https://github.com/Open-Transactions/opentxs-notary"
}

variable "sdk" {
  default = "opentransactions/alpine-sdk"
}

variable "runtime" {
  default = "opentransactions/alpine-runtime"
}

target "downloader" {
  dockerfile = "downloader"
  target = "downloader"
  tags = [
    "temp-opentxs-notary/downloader"
  ]
}

target "notary-download" {
  dockerfile = "download"
  target = "notary-download"
  contexts = {
    download = "target:downloader"
  }
  args = {
    OPENTXS_NOTARY_REPO = "${repo}"
    OPENTXS_NOTARY_TAG = "${tag}"
  }
  tags = [
    "temp-opentxs-notary/notary-download"
  ]
}

target "compile" {
  dockerfile = "compile"
  target = "compile"
  contexts = {
    notary-download = "target:notary-download"
  }
  args = {
    OPENTXS_SDK = "${sdk}"
    OPENTXS_VERSION = "${opentxs}"
  }
  tags = [
    "temp-opentxs-notary/compile"
  ]
}

target "default" {
  dockerfile = "run"
  target = "run"
  contexts = {
    compile = "target:compile"
  }
  args = {
    OPENTXS_RUNTIME = "${runtime}"
    OPENTXS_VERSION = "${opentxs}"
  }
  tags = [
    "opentransactions/notary:${tag}-${opentxs}-${target_arch}",
    "opentransactions/notary:latest-${target_arch}"
  ]
}
