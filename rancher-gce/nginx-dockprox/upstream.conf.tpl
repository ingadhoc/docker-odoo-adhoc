upstream {{cUpstreamServerName}} {
    server {{cUpstreamIp}}:{{cUpStreamPort}} weight=1 fail_timeout=300s;
}
