function _cloak_spec_4
set 1 $argv[1]
__fish_complete_path "$1"
end

function _cloak
    set COMP_LINE (commandline --cut-at-cursor)

    set COMP_WORDS
    echo $COMP_LINE | read --tokenize --array COMP_WORDS
    if string match --quiet --regex '.*\s$' $COMP_LINE
        set COMP_CWORD (math (count $COMP_WORDS) + 1)
    else
        set COMP_CWORD (count $COMP_WORDS)
    end

    set literals "--key" "--help" "-i" "-k" "encrypt" "decrypt" "hash" "-o" "--output" "--version" "ls" "-v" "-h" "--input"

    set descriptions
    set descriptions[3] "Specify the input path"
    set descriptions[4] "Specify the encryption key path"
    set descriptions[5] "Encrypt a file"
    set descriptions[6] "Decrypt a file"
    set descriptions[7] "Prints the hash of a file"
    set descriptions[8] "Specify the output path"
    set descriptions[11] "Lists the files considered in the cryptographic process"
    set descriptions[12] "Prints the project version"
    set descriptions[13] "Prints the help message"

    set literal_transitions
    set literal_transitions[1] "set inputs 2 11 12 13 5 6 7 10; set tos 2 3 2 2 4 5 6 2"
    set literal_transitions[3] "set inputs 3 14; set tos 8 8"
    set literal_transitions[4] "set inputs 3 14; set tos 9 9"
    set literal_transitions[5] "set inputs 3 14; set tos 13 13"
    set literal_transitions[6] "set inputs 3 14; set tos 8 8"
    set literal_transitions[7] "set inputs 1 4; set tos 8 8"
    set literal_transitions[11] "set inputs 9 8; set tos 8 8"
    set literal_transitions[12] "set inputs 9 8; set tos 10 10"

    set match_anything_transitions_from 10 8 13 9
    set match_anything_transitions_to 7 2 12 11

    set state 1
    set word_index 2
    while test $word_index -lt $COMP_CWORD
        set -- word $COMP_WORDS[$word_index]

        if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
            set --erase inputs
            set --erase tos
            eval $literal_transitions[$state]

            if contains -- $word $literals
                set literal_matched 0
                for literal_id in (seq 1 (count $literals))
                    if test $literals[$literal_id] = $word
                        set index (contains --index -- $literal_id $inputs)
                        set state $tos[$index]
                        set word_index (math $word_index + 1)
                        set literal_matched 1
                        break
                    end
                end
                if test $literal_matched -ne 0
                    continue
                end
            end
        end

        if set --query match_anything_transitions_from[$state] && test -n $match_anything_transitions_from[$state]
            set index (contains --index -- $state $match_anything_transitions_from)
            set state $match_anything_transitions_to[$index]
            set word_index (math $word_index + 1)
            continue
        end

        return 1
    end

    if set --query literal_transitions[$state] && test -n $literal_transitions[$state]
        set --erase inputs
        set --erase tos
        eval $literal_transitions[$state]
        for literal_id in $inputs
            if test -n $descriptions[$literal_id]
                printf '%s\t%s\n' $literals[$literal_id] $descriptions[$literal_id]
            else
                printf '%s\n' $literals[$literal_id]
            end
        end
    end

    set specialized_command_states 10 13 8 9
    set specialized_command_ids 4 4 4 4
    if contains $state $specialized_command_states
        set index (contains --index $state $specialized_command_states)
        set function_id $specialized_command_ids[$index]
        set function_name _cloak_spec_$function_id
        set --erase inputs
        set --erase tos
        set lines (eval $function_name $COMP_WORDS[$COMP_CWORD])
        for line in $lines
            printf '%s\n' $line
        end
    end

    return 0
end

complete --command cloak --no-files --arguments "(_cloak)"
