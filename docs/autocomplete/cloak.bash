_cloak_cmd_0 () {
    compgen -A file "$1"
}

_cloak () {
    if [[ $(type -t _get_comp_words_by_ref) != function ]]; then
        echo _get_comp_words_by_ref: function not defined.  Make sure the bash-completions system package is installed
        return 1
    fi

    local words cword
    _get_comp_words_by_ref -n "$COMP_WORDBREAKS" words cword

    declare -a literals=(decrypt -i --gitignore-ignore init ls ini generate hash encrypt --cloakignore-ignore --output -o --type autocomplete --help -v --version --key --input)
    declare -A literal_transitions=()
    literal_transitions[0]="([0]=6 [15]=1 [3]=1 [4]=2 [14]=1 [6]=3 [7]=4 [8]=5 [9]=1 [16]=1)"
    literal_transitions[2]="([1]=12 [9]=1 [2]=14 [18]=12)"
    literal_transitions[3]="([12]=15)"
    literal_transitions[4]="([1]=11 [18]=11)"
    literal_transitions[5]="([1]=16 [18]=16)"
    literal_transitions[6]="([1]=9 [18]=9)"
    literal_transitions[8]="([17]=11)"
    literal_transitions[10]="([10]=7 [11]=7 [17]=11)"
    literal_transitions[13]="([2]=14 [9]=1)"
    literal_transitions[14]="([9]=1)"
    literal_transitions[15]="([5]=1 [13]=1)"
    literal_transitions[17]="([10]=18 [2]=1 [11]=18)"
    literal_transitions[19]="([2]=1)"
    declare -A match_anything_transitions=([9]=10 [18]=19 [12]=13 [7]=8 [16]=17 [11]=1)
    declare -A subword_transitions

    local state=0
    local word_index=1
    while [[ $word_index -lt $cword ]]; do
        local word=${words[$word_index]}

        if [[ -v "literal_transitions[$state]" ]]; then
            declare -A state_transitions
            eval "state_transitions=${literal_transitions[$state]}"

            local word_matched=0
            for literal_id in $(seq 0 $((${#literals[@]} - 1))); do
                if [[ ${literals[$literal_id]} = "$word" ]]; then
                    if [[ -v "state_transitions[$literal_id]" ]]; then
                        state=${state_transitions[$literal_id]}
                        word_index=$((word_index + 1))
                        word_matched=1
                        break
                    fi
                fi
            done
            if [[ $word_matched -ne 0 ]]; then
                continue
            fi
        fi

        if [[ -v "match_anything_transitions[$state]" ]]; then
            state=${match_anything_transitions[$state]}
            word_index=$((word_index + 1))
            continue
        fi

        return 1
    done

    declare -A literal_transitions_level_0=([2]="1 9 2" [13]="2 9" [0]="0 15 3 4 14 6 7 8 9" [19]="2" [6]="1" [15]="5 13" [5]="1" [17]="2 11" [3]="12" [10]="11 17" [14]="9" [4]="1" [8]="17")
    declare -A literal_transitions_level_1=([17]="10" [5]="18" [10]="10" [4]="18" [2]="18" [0]="16" [6]="18")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([16]="0" [9]="0" [18]="0" [12]="0" [7]="0" [11]="0")
    declare -A specialized_commands_level_1=()

    local -a matches=()
    local max_fallback_level=1
    local prefix="${words[$cword]}"
    for (( fallback_level=0; fallback_level <= max_fallback_level; fallback_level++ )) {
       eval "declare literal_transitions_name=literal_transitions_level_${fallback_level}"
       eval "declare -a transitions=(\${$literal_transitions_name[$state]})"
       for literal_id in "${transitions[@]}"; do
           local literal="${literals[$literal_id]}"
           if [[ $literal = "${prefix}"* ]]; then
               matches+=("$literal ")
           fi
       done

       eval "declare subword_transitions_name=subword_transitions_level_${fallback_level}"
       eval "declare -a transitions=(\${$subword_transitions_name[$state]})"
       for subword_id in "${transitions[@]}"; do
           readarray -t -O "${#matches[@]}" matches < <(_cloak_subword_$subword_id complete "$prefix")
       done

       eval "declare commands_name=commands_level_${fallback_level}"
       eval "declare -a transitions=(\${$commands_name[$state]})"
       for command_id in "${transitions[@]}"; do
           local completions=()
           readarray -t completions < <(_cloak_cmd_$command_id "$prefix" | cut -f1)
           for item in "${completions[@]}"; do
               if [[ $item = "${prefix}"* ]]; then
                   matches+=("$item")
               fi
           done
       done

       eval "declare specialized_commands_name=specialized_commands_level_${fallback_level}"
       eval "declare -a transitions=(\${$specialized_commands_name[$state]})"
       for command_id in "${transitions[@]}"; do
           local completions=()
           readarray -t completions < <(_cloak_cmd_"${command_id}" "$prefix" | cut -f1)
           for item in "${completions[@]}"; do
               if [[ $item = "${prefix}"* ]]; then
                   matches+=("$item")
               fi
           done
       done

       if [[ ${#matches[@]} -gt 0 ]]; then
           local shortest_suffix="$prefix"
           for ((i=0; i < ${#COMP_WORDBREAKS}; i++)); do
               local char="${COMP_WORDBREAKS:$i:1}"
               local candidate=${prefix##*$char}
               if [[ ${#candidate} -lt ${#shortest_suffix} ]]; then
                   shortest_suffix=$candidate
               fi
           done
           local superfluous_prefix=""
           if [[ "$shortest_suffix" != "$prefix" ]]; then
               local superfluous_prefix=${prefix%$shortest_suffix}
           fi
           COMPREPLY=("${matches[@]#$superfluous_prefix}")
           break
       fi
    }

    return 0
}

complete -o nospace -F _cloak cloak
