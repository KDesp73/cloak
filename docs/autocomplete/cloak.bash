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

    declare -a literals=(-i encrypt decrypt ini ls hash generate --type -o --output autocomplete --help -v --version --gitignore-ignore --key --input)
    declare -A literal_transitions=()
    literal_transitions[0]="([12]=1 [1]=2 [2]=3 [4]=4 [11]=1 [6]=5 [5]=6 [13]=1)"
    literal_transitions[2]="([0]=15 [16]=15)"
    literal_transitions[3]="([0]=13 [16]=13)"
    literal_transitions[4]="([0]=12 [14]=1 [16]=12)"
    literal_transitions[5]="([7]=10)"
    literal_transitions[6]="([0]=9 [16]=9)"
    literal_transitions[8]="([15]=9)"
    literal_transitions[10]="([3]=1 [10]=1)"
    literal_transitions[11]="([9]=12 [14]=1 [8]=12)"
    literal_transitions[14]="([9]=7 [8]=7 [15]=9)"
    literal_transitions[16]="([14]=1)"
    declare -A match_anything_transitions=([9]=1 [15]=11 [13]=14 [12]=16 [7]=8)
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

    declare -A literal_transitions_level_0=([2]="0" [0]="12 1 2 4 11 6 5" [11]="14 8" [10]="3 10" [6]="0" [5]="7" [16]="14" [3]="0" [4]="0 14" [14]="8 15" [8]="15")
    declare -A literal_transitions_level_1=([14]="9" [4]="16" [3]="16" [2]="16" [0]="13" [11]="9" [6]="16")
    declare -A subword_transitions_level_0=()
    declare -A subword_transitions_level_1=()
    declare -A commands_level_0=()
    declare -A commands_level_1=()
    declare -A specialized_commands_level_0=([15]="0" [9]="0" [13]="0" [12]="0" [7]="0")
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
